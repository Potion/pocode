//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poGeometryMask.h"
#include "Helpers.h"

poGeometryMask::poGeometryMask(poShape2D *shape)
:	shape(shape)
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

void poGeometryMask::doSetUp( poObject* obj ) {
	if(shape)
		startMasking(shape);
}

void poGeometryMask::doSetDown( poObject* obj ) {
	if(shape)
		stopMasking();
}



