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


poGeometryMask::poGeometryMask(poRect r) {
	points.push_back(r.origin);
	points.push_back(poPoint(r.origin.x+r.size.x, r.origin.y));
	points.push_back(r.origin+r.size);
	points.push_back(poPoint(r.origin.x, r.origin.y+r.size.y));
}

void poGeometryMask::setUp( poObject* obj ) {
	glPushAttrib(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawPoints(points);
	
	glStencilFunc(GL_EQUAL, 1, 1);
}

void poGeometryMask::setDown( poObject* obj ) {
	glPopAttrib();
}



