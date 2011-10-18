//
//  poFBO.cpp
//  potionCode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poFBO.h"
#include "poOpenGLState.h"

poFBOConfig::poFBOConfig() {}

poFBOConfig &poFBOConfig::setColorConfig(const poTextureConfig &config) {
	colorConfig = config;
	return *this;
}

poFBO::poFBO(uint w, uint h, const poFBOConfig &c)
:	width(w)
,	height(h)
,	config(c)
,	cam(new poCamera2D())
{
	uids[0] = 0;
	cam->fixedSize(true, poPoint(w,h));
	setup();
}

poFBO::~poFBO() {
	if(isValid())
		cleanup();
}

bool poFBO::isValid() const {
	return uids[0] > 0;
}

void poFBO::setCamera(poCamera *camera) {
	delete cam;
	cam = camera->copy();
}

void poFBO::reset(uint w, uint h, const poFBOConfig &c) {
	if(isValid())
		cleanup();
	
	width = w;
	height = h;
	config = c;
	setup();
}

// retrieve this texture to draw the FBO
poTexture *poFBO::colorTexture() const {
	return colorTex;
}

poTexture *poFBO::depthTexture() const {
	
}

void poFBO::doSetUp(poObject* obj) {
	cam->setUp(obj);
	glBindFramebuffer(GL_FRAMEBUFFER, uids[0]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poFBO::doSetDown(poObject* obj) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	cam->setDown(obj);
}

void poFBO::setup() {
	glGenFramebuffers(1, uids);
	glBindFramebuffer(GL_FRAMEBUFFER, uids[0]);
	
	colorTex = new poTexture(width, height, NULL, config.colorConfig);
	glBindTexture(GL_TEXTURE_2D, colorTex->uid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex->uid, 0);
	
	GLenum ok = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(ok != GL_FRAMEBUFFER_COMPLETE) {
		printf("framebuffer setup failed: %d\n", ok);
		cleanup();
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void poFBO::cleanup() {
	delete colorTex; colorTex = NULL;

	glDeleteFramebuffers(1, &uids[0]);
	uids[0] = 0;
}





