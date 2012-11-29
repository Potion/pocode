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
//  FBO.h
//  pocode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poCamera.h"
#include "poTexture.h"
#include "poObjectModifier.h"

namespace po {
    struct FBOConfig {
        FBOConfig();
        FBOConfig &setNumMultisamples(uint numMultisamples);
        FBOConfig &setNumColorBuffers(uint numColorBuffers);
        FBOConfig &setColorBufferConfig(TextureConfig config);
        
        uint numColorBuffers;
        uint numMultisamples;
        TextureConfig textureConfig;
    };

    class FBO : public ObjectModifier {
    public:
        FBO(uint w, uint h);
        FBO(uint w, uint h, const FBOConfig &config);
        virtual ~FBO();
        
        virtual ObjectModifier *copy();
        
        bool isValid() const;
        Camera* getCamera() const;
        void setCamera(Camera *cam);
        void reset(uint w, uint h, const FBOConfig &config);

        // retrieve this texture to draw the FBO
        Texture *getColorTexture(uint idx=0) const;
        Texture *getColorTextureCopy(uint idx=0);
        Texture *getDepthTexture() const;
        
        // after the fbo is created
        uint width, height;
        FBOConfig config;
        
    protected:
        void clone(FBO *fbo);
        
        virtual void doSetUp(Object*);
        virtual void doSetDown(Object*);

        void setup();
        void cleanup();
        
    private:
        std::vector<GLuint> framebuffers;
        std::vector<GLuint> renderbuffers;
        std::vector<Texture*> colorbuffers;
        
        Camera *cam;
        
        bool multisampling;
    };
} /* End po Namespace */