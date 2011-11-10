//
//  LineExtruder.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#pragma once

#include "poShape2D.h"

struct poExtrudedLineSeg { 
	poPoint p1, p2, p3, p4;
	poStrokePlacementProperty place;
	
	poExtrudedLineSeg();
	poExtrudedLineSeg(poPoint a, poPoint b, float w, poStrokePlacementProperty place);
	poExtrudedLineSeg(poPoint ul, poPoint ll, poPoint ur, poPoint lr);
};

// angle ABC but with extruded line segments
float angleBetweenSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2);
// returns true if top is on the outside
bool combineExtrudedLineSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2, poPoint *top, poPoint *bottom);
// layout the wide stroke for the joint
void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, poStrokeJoinProperty join, float stroke_width);
