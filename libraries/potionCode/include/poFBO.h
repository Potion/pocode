//
//  poFBO.h
//  potionCode
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
	
	uint numMultisamples;
};

class poFBO : public poObjectModifier
{
public:
	poFBO(uint w, uint h);
	poFBO(uint w, uint h, const poFBOConfig &config);
	virtual ~poFBO();
	
	bool isValid() const;
	void setCamera(poCamera *cam);
	void reset(uint w, uint h, const poFBOConfig &config);

	// retrieve this texture to draw the FBO
	poTexture *colorTexture() const;
	poTexture *depthTexture() const;
	
	// after the fbo is created
	uint width, height;
	poFBOConfig config;
	
protected:
	virtual void doSetUp(poObject*);
	virtual void doSetDown(poObject*);

	void setup();
	void cleanup();
	
private:
	std::vector<GLuint> framebuffers;
	std::vector<GLuint> renderbuffers;
	
	poCamera *cam;
	poTexture *colorTex;
	
	bool multisampling;
};


