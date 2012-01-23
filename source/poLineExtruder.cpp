//
//  LineExtruder.cpp
//  pocode
//
//  Created by Joshua Fisher on 6/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poLineExtruder.h"
#include "poMath.h"

poExtrudedLineSeg::poExtrudedLineSeg()
{}

poExtrudedLineSeg::poExtrudedLineSeg(poPoint a, poPoint b, float w, poStrokePlacementProperty place) {
	poPoint diff = b - a;
	float angle = atan2(diff.y, diff.x);
	
	this->place = place;
	
	float offset1 = 0;
	float offset2 = 0;
	
	switch(place) {
		case PO_STROKE_PLACE_CENTER:
			offset1 = offset2 = w / 2.f;
			break;
		case PO_STROKE_PLACE_INSIDE:
			offset1 = w;
			offset2 = 0.f;
			break;
		case PO_STROKE_PLACE_OUTSIDE:
			offset1 = 0.f;
			offset2 = w;
			break;
	};
	
	float c1 = cosf(angle + M_HALF_PI) * offset1;
	float s1 = sinf(angle + M_HALF_PI) * offset1;
	float c2 = cosf(angle - M_HALF_PI) * offset2;
	float s2 = sinf(angle - M_HALF_PI) * offset2;
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
	poPoint p1, p2, p3;
	
	switch(seg1.place) {
		case PO_STROKE_PLACE_CENTER:
			p1 = (seg1.p2 + seg1.p1) / 2.f;
			p2 = (seg1.p4 + seg1.p3) / 2.f;
			p3 = (seg2.p4 + seg2.p3) / 2.f;
			break;
		case PO_STROKE_PLACE_INSIDE:
			p1 = seg1.p1;
			p2 = seg1.p3;
			p3 = seg2.p3;
			break;
		case PO_STROKE_PLACE_OUTSIDE:
			p1 = seg1.p2;
			p2 = seg1.p4;
			p3 = seg2.p4;
			break;
	}
	
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

void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, poStrokeJoinProperty join, float stroke_width) {
	poPoint top, bottom;
	poPoint p1, p2, p3, p4, tmp;
	
	bool top_outside = combineExtrudedLineSegments(seg1, seg2, &top, &bottom);
	poPoint joint = (seg1.p4 + seg1.p3) / 2.f;
	
	switch(join) {
		case PO_STROKE_JOIN_MITRE:
			if(top_outside) {
				stroke.push_back(top);
				stroke.push_back(bottom);
			}
			else {
				stroke.push_back(bottom);
				stroke.push_back(top);
			}
			break;
			
		case PO_STROKE_JOIN_BEVEL:
			if(top_outside) {
				stroke.push_back(seg1.p3);
				stroke.push_back(bottom);
				stroke.push_back(seg2.p1);
				stroke.push_back(bottom);
			}
			else {
				stroke.push_back(top);
				stroke.push_back(seg1.p4);
				stroke.push_back(top);
				stroke.push_back(seg2.p2);
			}
			break;
			
		case PO_STROKE_JOIN_ROUND:
		{
			float halfW = stroke_width / 2.f;
			
			if(top_outside) {
				p1 = bottom;
				p2 = seg1.p3;
				p3 = joint;
				p4 = seg2.p1;
				
				stroke.push_back(p2);
				stroke.push_back(p1);
				
				float a1 = angleBetweenPoints(p2, p3, p4);
				
				poPoint diff = p2 - p3;
				float a2 = atan2f(diff.y, diff.x);
				
				float step = a1 / 9.f;
				float a = a2;
				
				for(int j=0; j<10; j++) {
					tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
					stroke.push_back(tmp+p3);
					stroke.push_back(p3);
					a += step;
				}
				
				stroke.push_back(p4);
				stroke.push_back(p1);
			}
			else {
				p1 = top;
				p2 = seg1.p4;
				p3 = joint;
				p4 = seg2.p2;
				
				stroke.push_back(p1);
				stroke.push_back(p2);
				
				float a1 = angleBetweenPoints(p2, p3, p4);
				
				poPoint diff = p2 - p3;
				float a2 = atan2f(diff.y, diff.x);
				
				float step = a1 / 9.f;
				float a = a2;
				
				for(int j=0; j<10; j++) {
					tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
					stroke.push_back(p3);
					stroke.push_back(tmp+p3);
					a += step;
				}
				
				stroke.push_back(p1);
				stroke.push_back(p4);
			}
			
			break;
		}
	}
}