//
//  LineExtruder.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "LineExtruder.h"
#include "poMath.h"

poExtrudedLineSeg::poExtrudedLineSeg()
{}

poExtrudedLineSeg::poExtrudedLineSeg(poPoint a, poPoint b, float w) {
	poPoint diff = b - a;
	float angle = atan2(diff.y, diff.x);
	float halfW = w / 2.f;
	float c1 = cosf(angle + M_HALF_PI) * halfW;
	float s1 = sinf(angle + M_HALF_PI) * halfW;
	float c2 = cosf(angle - M_HALF_PI) * halfW;
	float s2 = sinf(angle - M_HALF_PI) * halfW;
	p1 = a + poPoint(c1,s1);
	p2 = a + poPoint(c2,s2);
	p3 = b + poPoint(c1,s1);
	p4 = b + poPoint(c2,s2);
}

poExtrudedLineSeg::poExtrudedLineSeg(poPoint ul, poPoint ll, poPoint ur, poPoint lr)
:	p1(ul)
,	p2(ll)
,	p3(ur)
,	p4(lr)
{}

float angleBetweenSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2) {
	poPoint p1 = (seg1.p2 + seg1.p1) / 2.f;
	poPoint p2 = (seg1.p4 + seg1.p3) / 2.f;
	poPoint p3 = (seg2.p4 + seg2.p3) / 2.f;
	
	return angleBetweenPoints(p1, p2, p3);
}

bool combineExtrudedLineSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2, poPoint *top, poPoint *bottom) {
	poPoint i1, i2, i3, i4;
	
	float angle = angleBetweenSegments(seg1, seg2);
	
	if(compare(angle,0.f)) {
		*top = seg1.p3;
		*bottom = seg2.p4;
	}
	else {
		rayIntersection(poRay(seg1.p2, seg1.p4-seg1.p2),
						poRay(seg2.p4, seg2.p2-seg2.p4),
						&i1, &i2);
		
		rayIntersection(poRay(seg1.p1, seg1.p3-seg1.p1),
						poRay(seg2.p3, seg2.p1-seg2.p3),
						&i3, &i4);
		
		*top = i3;
		*bottom = i1;
	}
	
	return angle > 0.f;
}
