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

namespace po {
    struct ExtrudedLineSeg {
        Point p1, p2, p3, p4;
        StrokePlacementProperty place;
        
        ExtrudedLineSeg();
        ExtrudedLineSeg(Point a, Point b, float w, StrokePlacementProperty place);
        ExtrudedLineSeg(Point ul, Point ll, Point ur, Point lr);
    };

    // angle ABC but with extruded line segments
    float angleBetweenSegments(ExtrudedLineSeg seg1, ExtrudedLineSeg seg2);
    // returns true if top is on the outside
    bool combineExtrudedLineSegments(ExtrudedLineSeg seg1, ExtrudedLineSeg seg2, Point *top, Point *bottom);
    // layout the wide stroke for the joint
    void makeStrokeForJoint(std::vector<Point> &stroke, ExtrudedLineSeg &seg1, ExtrudedLineSeg &seg2, StrokeJoinProperty join, float stroke_width);
} /* End po Namespace */