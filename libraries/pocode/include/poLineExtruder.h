/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  LineExtruder.h
//  pocode
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
