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
//  poFBO.h
//  pocode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poCamera.h"
#include "poTexture.h"
#include "poObjectModifier.h"

struct poFBOConfig {
	poFBOConfig();
	poFBOConfig &setNumMultisamples(uint numMultisamples);
	poFBOConfig &setNumColorBuffers(uint numColorBuffers);
	poFBOConfig &setColorBufferConfig(poTextureConfig config);
	poFBOConfig &setDepthStencil(bool);
	
	uint numColorBuffers;
	uint numMultisamples;
	poTextureConfig textureConfig;
	bool hasDepthStencil;
};

class poFBO : public poObjectModifier {
public:
	poFBO(uint w, uint h);
	poFBO(uint w, uint h, const poFBOConfig &config);
	virtual ~poFBO();
	
	virtual poObjectModifier *copy();
	
	bool isValid() const;
	poCamera* getCamera() const;
	void setCamera(poCamera *cam);
	void reset(uint w, uint h, const poFBOConfig &config);

	// retrieve this texture to draw the FBO
	poTexture *getColorTexture(uint idx=0) const;
	poTexture *getColorTextureCopy(uint idx=0);
	poTexture *getDepthTexture() const;
	
	// after the fbo is created
	uint width, height;
	poFBOConfig config;
	
protected:
	void clone(poFBO *fbo);
	
	virtual void doSetUp(poObject*);
	virtual void doSetDown(poObject*);

	void setup();
	void cleanup();
	
private:
	std::vector<GLuint> framebuffers;
	std::vector<GLuint> renderbuffers;
	std::vector<poTexture*> colorbuffers;
	poTexture* depthStencil;
	
	poCamera *cam;
	
	bool multisampling;
};
