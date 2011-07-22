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

void poGeometryMask::doSetUp( poObject* obj ) {
	startMasking(shape);
}

void poGeometryMask::doSetDown( poObject* obj ) {
	stopMasking();
}



