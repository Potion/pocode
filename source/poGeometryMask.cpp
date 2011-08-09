//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMask.h"
#include "Helpers.h"

poGeometryMask::poGeometryMask(poShape2D *shape, bool clearsStencil)
:	shape(shape)
,	clears_stencil(clearsStencil)
{}

poGeometryMask::~poGeometryMask() {
	if(shape)
		delete shape;
}

void poGeometryMask::setShape(poShape2D *s) {
	if(shape)
		delete shape;
	shape = s;
	enabled(shape != NULL);
}

bool poGeometryMask::pointInside(poPoint p) {
	
}

void poGeometryMask::doSetUp( poObject* obj ) {
	if(shape) {
		if(clears_stencil)
			glClear(GL_STENCIL_BUFFER_BIT);
		startMasking(shape);
	}
}

void poGeometryMask::doSetDown( poObject* obj ) {
	if(shape)
		stopMasking();
}



