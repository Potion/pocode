//
//  poMath.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

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

poRay::poRay(poPoint o, poPoint d)
:	origin(o)
,	dir(normalize(d))
{}

float determinant(poPoint row1, poPoint row2, poPoint row3) {
	return (row1.x*row2.y*row3.z + row1.y*row2.z*row3.x + row1.z*row2.x*row3.y -
			row1.x*row2.z*row3.y - row1.y*row2.x*row3.z - row1.z*row2.y*row3.x);
}

bool rayIntersection(poRay r1, poRay r2, poPoint *p1, poPoint *p2) {
	poPoint c1 = r1.dir.cross(r2.dir);
	float len_sqr = c1.lengthSquared();
	
	if(compare(len_sqr, 0.f)) {
		return false;
	}
	
	poPoint diff = r2.origin - r1.origin;
	float t1 = determinant(diff, r2.dir, c1) / len_sqr;
	float t2 = determinant(diff, r1.dir, c1) / len_sqr;
	
	*p1 = r1.origin + r1.dir*t1;
	*p2 = r2.origin + r2.dir*t2;
	
	return *p1 == *p2;
}

float angleBetweenPoints(poPoint a, poPoint b, poPoint c) {
	poPoint ab = a - b;
	poPoint cb = c - b;
	
	// perp dot product
	float ret = atan2f(ab.x*cb.y - ab.y*cb.x, ab.x*cb.x + ab.y*cb.y);

	return ret;
}

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
