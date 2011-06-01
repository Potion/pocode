//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMaskModifier.h"
#include "poSimpleDrawing.h"

poImageMask::poImageMask(poTexture *tex)
:	texture(tex->copy())
{}

poImageMask::poImageMask(const std::string &str) {
	poImage *img = new poImage(str, IMAGE_8);
	texture = new poTexture(GL_ALPHA, img->width(), img->height(), img->storageSize(), img->pixels());
	delete img;
}

poImageMask::~poImageMask() {
	delete texture;
}

void poImageMask::setUp( poObject* obj ) {
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	
	glBlendFuncSeparate(GL_ZERO, GL_DST_COLOR, GL_SRC_COLOR, GL_ZERO);
	drawRect(texture);
	
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poImageMask::setDown( poObject* obj ) {
	glPopAttrib();
}
