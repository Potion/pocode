//
//  LineExtruder.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#pragma once

#include "poPoint.h"
#include "poEnums.h"

struct poExtrudedLineSeg { 
	poPoint p1, p2, p3, p4;
	StrokePlacementProperty place;
	
	poExtrudedLineSeg();
	poExtrudedLineSeg(poPoint a, poPoint b, float w, StrokePlacementProperty place);
	poExtrudedLineSeg(poPoint ul, poPoint ll, poPoint ur, poPoint lr);
};

// angle ABC but with extruded line segments
float angleBetweenSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2);
// returns true if top is on the outside
bool combineExtrudedLineSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2, poPoint *top, poPoint *bottom);