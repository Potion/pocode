//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMaskModifier.h"
#include "poSimpleDrawing.h"
#include "Helpers.h"
#include "poShape2D.h"

poImageMask::poImageMask(poTexture *tex)
:	texture(tex->copy())
{}

poImageMask::poImageMask(const std::string &str) {
	poImage img(str, IMAGE_8);
//	texture = new poTexture(img);
	texture = new poTexture(GL_ALPHA, img.width(), img.height(), img.pitch(), img.pixels());
}

poImageMask::~poImageMask() {
	delete texture;
}

void poImageMask::setUp( poObject* obj ) {
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	
	glClear(GL_STENCIL_BUFFER_BIT);
	
	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
	drawRect(texture);
	
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poImageMask::setDown( poObject* obj ) {
	glPopAttrib();
//	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glDisable(GL_DEPTH_TEST);
//	glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_DST_COLOR, GL_ZERO);
//	drawRect(texture);
//	glPopAttrib();
}


poGeometryMask::poGeometryMask(poShape2D *shape)
:	shape(shape)
{}

void poGeometryMask::setUp( poObject* obj ) {
	startMasking(shape);
}

void poGeometryMask::setDown( poObject* obj ) {
	stopMasking();
}



