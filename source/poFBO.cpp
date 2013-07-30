/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  FBO.cpp
//  pocode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poFBO.h"
#include "poTexture.h"
#include "poOpenGLState.h"
#include "poApplication.h"

#include <boost/foreach.hpp>

#define ASSERTGL() { int err = glGetError(); if(err != GL_NO_ERROR) {printf("x%06x\n", err); assert(false);} }

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ========================== Class: FBOConfig ==================================
    #pragma mark - FBOConfig -

    FBOConfig::FBOConfig()
    :	numMultisamples(0)
    ,	numColorBuffers(1)
    ,	textureConfig(GL_RGBA)
	,	hasDepthStencil(false)
    {}
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setNumMultisamples(uint nm) {
        numMultisamples = nm;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setNumColorBuffers(uint ncb) {
        numColorBuffers = ncb;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setColorBufferConfig(TextureConfig c) {
        textureConfig = c;
        return *this;
    }
    
    
	//------------------------------------------------------------------------
	FBOConfig &FBOConfig::setDepthStencil(bool b) {
		hasDepthStencil = b;
		return *this;
	}
    
    
    // -----------------------------------------------------------------------------------
    // ========================== Class: FBO =============================================
    #pragma mark - FBO -

    FBO::FBO(uint w, uint h)
    :	width(w)
    ,	height(h)
    ,	cam(new Camera2D())
    ,	multisampling(false)
	,	depthStencil(NULL)
    {
        cam->setFixedSize(true, Point(w,h));
        setup();
    }

    FBO::FBO(uint w, uint h, const FBOConfig &c)
    :	width(w)
    ,	height(h)
    ,	config(c)
    ,	cam(new Camera2D())
    ,	multisampling(false)
	,	depthStencil(NULL)
    {
        cam->setFixedSize(true, Point(w,h));
        setup();
    }

    FBO::~FBO() {
        if(isValid())
            cleanup();
    }
    
    
    //------------------------------------------------------------------------
    ObjectModifier *FBO::copy() {
        FBO *fbo = new FBO(width, height, config);
        clone(fbo);
        return fbo;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::clone(FBO* obj) {
        obj->setCamera(cam);
        ObjectModifier::clone(obj);
    }
    
    
    //------------------------------------------------------------------------
    bool FBO::isValid() const {
        return !framebuffers.empty() && framebuffers[0] > 0 && !colorbuffers.empty();
    }
    
    
    //------------------------------------------------------------------------
    Camera* FBO::getCamera() const {
        return cam;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::setCamera(Camera *camera) {
        delete cam;
        cam = (Camera*)camera->copy();
    }
    
    
    //------------------------------------------------------------------------
    void FBO::reset(uint w, uint h, const FBOConfig &c) {
        if(isValid())
            cleanup();
        
        width = w;
        height = h;
        config = c;
        setup();
    }
    
    
    //------------------------------------------------------------------------
    // retrieve this texture to draw the FBO
    Texture *FBO::getColorTexture(uint idx) const {
        return colorbuffers[idx];
    }
    
    
    //------------------------------------------------------------------------
    Texture *FBO::getColorTextureCopy(uint idx) {
        Texture *col = colorbuffers[idx];
        Texture *tex = new Texture(width, height, NULL, col->getConfig());
        
        po::saveTextureState();
        glBindTexture(GL_TEXTURE_2D, tex->getUid());
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        po::restoreTextureState();
        
        return tex;
    }
    
    
    //------------------------------------------------------------------------
    Texture *FBO::getDepthTexture() const {
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::doSetUp(Object* obj) {
		ASSERTGL()
        po::saveTextureState();
		ASSERTGL()
        po::saveViewport();
		ASSERTGL()
        po::setViewport(0,0,width/po::getScale(),height/po::getScale());
		ASSERTGL()
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
		ASSERTGL()
        cam->setUp(obj);
    }
    
    
    //------------------------------------------------------------------------
    void FBO::doSetDown(Object* obj) {
        cam->setDown(obj);
        
        if(multisampling) {
        #ifdef OPENGL_ES
            #ifdef POTION_IOS
                glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, framebuffers[1]);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, framebuffers[0]);
                glResolveMultisampleFramebufferAPPLE();
            #else
                #warning non-ios opengl es fbo multisample implementation incomplete
            #endif
        #else
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[1]);
            glBlitFramebuffer(0,0,width,height, 0,0,width,height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        #endif
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ASSERTGL()
        po::restoreTextureState();
        po::restoreViewport();
		ASSERTGL()
    }
    
#ifdef POTION_IOS
#define glRenderbufferStorageMultisample glRenderbufferStorageMultisampleAPPLE
#define GL_RGBA8 GL_RGBA8_OES
#endif
    
    //------------------------------------------------------------------------
    void FBO::setup() {
        int maxSamples =  po::maxFBOSamples();
		
		glGetError();
		
		if(config.numMultisamples && maxSamples) {
			if(config.numMultisamples > maxSamples)
				config.numMultisamples = maxSamples;
			
			multisampling = true;
			// we need 2 different framebuffers
			framebuffers.resize(2);
			glGenFramebuffers(2, &framebuffers[0]);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
			
			renderbuffers.resize(2);
			glGenRenderbuffers(2, &renderbuffers[0]);
			
			// this is the multisample render buffer
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[0]);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, config.numMultisamples, GL_RGBA8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffers[0]);
			
			// same for depth/stencil
			if(config.hasDepthStencil) {
				glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[1]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, config.numMultisamples, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffers[1]);
			}
			
			// unbind render buffer
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
			// attach it to the second framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);
		}
		else {
			// we only need the one framebuffer
			framebuffers.resize(1);
			glGenFramebuffers(1, &framebuffers[0]);
			ASSERTGL();
			
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
			ASSERTGL();
		}
		
		colorbuffers.clear();
		for(int i=0; i<config.numColorBuffers; i++) {
			colorbuffers.push_back(new Texture(width,height,NULL,config.textureConfig));
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colorbuffers[i]->getUid(), 0);
			ASSERTGL()
		}
		
		if(config.hasDepthStencil) {
			depthStencil = new Texture(width, height, NULL, TextureConfig(GL_DEPTH_STENCIL).setInternalFormat(GL_DEPTH24_STENCIL8).setType(GL_UNSIGNED_INT_24_8));
			ASSERTGL()
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencil->getUid(), 0);
			ASSERTGL()
		}
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		
		glBindTexture(GL_TEXTURE_2D, 0);
		ASSERTGL()
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ASSERTGL()
    }
    
    
    //------------------------------------------------------------------------
    void FBO::cleanup() {
        BOOST_FOREACH(Texture *tex, colorbuffers)
            delete tex;
        colorbuffers.clear();
		
		if(depthStencil) {
			delete depthStencil;
			depthStencil = NULL;
		}

        // make sure none of this is bound right now
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(framebuffers.size(), &framebuffers[0]);
        glDeleteRenderbuffers(renderbuffers.size(), &renderbuffers[0]);
		ASSERTGL()
        framebuffers[0] = 0;
    }
}