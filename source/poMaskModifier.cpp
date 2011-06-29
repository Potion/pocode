//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMaskModifier.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"

#include <boost/lambda/lambda.hpp>

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


poGeometryMask::poGeometryMask(poRect r) {
	points.push_back(r.origin);
	points.push_back(poPoint(r.origin.x+r.size.x, r.origin.y));
	points.push_back(r.origin+r.size);
	points.push_back(poPoint(r.origin.x, r.origin.y+r.size.y));
}

poGeometryMask::poGeometryMask(const std::vector<poPoint> &pts) {
	points.assign(pts.begin(), pts.end());
}

poGeometryMask::poGeometryMask(poShape2D *shape) {
	const std::vector<poPoint> &pts = shape->getPoints();
	points.assign(pts.begin(), pts.end());
	
	using namespace boost::lambda;
	using boost::lambda::_1;
	std::transform(points.begin(), points.end(), points.begin(), ret<poPoint>(_1 + shape->position()));
}

void poGeometryMask::setUp( poObject* obj ) {
	glPushAttrib(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawPoints(points);
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
}

void poGeometryMask::setDown( poObject* obj ) {
	glPopAttrib();
}



