//
//  poPath.cpp
//  pathTest
//
//  Created by Joshua Fisher on 11/12/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poPath.h"

poPath& poPath::start(poPoint startPoint) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_MOVETO,0,1));
	points.clear();
	points.push_back(startPoint);
	return *this;
}
poPath& poPath::lineTo(poPoint endPoint) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_LINETO,points.size(),1));
	points.push_back(endPoint);
	return *this;
}
poPath& poPath::arcTo(poPoint center, float startAngle, float endAngle, bool clockwise) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_ARCTO,points.size(),2));
	points.push_back(center);
	points.push_back(poPoint(startAngle, endAngle, clockwise));
	return *this;
}
poPath& poPath::curveTo(poPoint endPoint, poPoint control) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_CUBETO,points.size(),2));
	points.push_back(endPoint);
	points.push_back(control);
	return *this;
}
poPath& poPath::curveTo(poPoint endPoint, poPoint control1, poPoint control2) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_QUADTO,points.size(),3));
	points.push_back(endPoint);
	points.push_back(control1);
	points.push_back(control2);
	return *this;
}
poPath& poPath::close() {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_CLOSE,points.size(),0));
	return *this;
}
uint poPath::getNumSegments() const {
	return segments.size();
}
poPathSegmentType poPath::getSegmentType(uint idx) const {
	return segments[idx].type;
}
bool poPath::isSegmentType(uint idx, poPathSegmentType type) const {
	return segments[idx].type == type;
}
void poPath::modifyLineTo(uint idx, poPoint point) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_LINETO)) {
		points[segments[idx].startIdx] = point;
	}
}
void poPath::modifyArcTo(uint idx, poPoint center, float startAngle, float endAngle, bool clockwise) {
	segment_storage &storage = segments[idx];
	
	if(storage.count != 2) {
	}
	points[segments[idx].startIdx] = center;
	points[segments[idx].startIdx+1] = poPoint(startAngle, endAngle, clockwise);
}
void poPath::modifyQuadTo(uint idx, poPoint point, poPoint control) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_QUADTO)) {
		
	}
}
void poPath::modifyCubeTo(uint idx, poPoint point, poPoint control1, poPoint control2) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_CUBETO)) {
		
	}
}
void poPath::removeSegment(uint idx) {
	segment_storage &storage =  segments[idx];

	std::vector<poPoint>::iterator iter = points.begin()+storage.startIdx;
	short count = storage.count;
	points.erase(iter, iter+count);
	
	segments.erase(segments.begin()+idx);
	for(int i=idx; i<segments.size(); i++) {
		segments[i].startIdx -= count;
	}
}
bool poPath::isEmpty() const {
	return segments.empty();
}
bool poPath::isOpen() const {
	return !isEmpty() && segments.back().type == PO_PATH_SEGMENT_CLOSE;
}
std::vector<poPoint> poPath::generatePoints() const {
	std::vector<poPoint> response;
	
	int rez = 20;
	for(int i=0; i<segments.size(); i++) {
		bool bail = false;
		switch(segments[i].type) {
			case PO_PATH_SEGMENT_MOVETO:
			case PO_PATH_SEGMENT_LINETO:
				response.push_back(points[segments[i].startIdx]);
				break;
				
			case PO_PATH_SEGMENT_ARCTO: {
				poPoint center = points[segments[i].startIdx];
				poPoint data = points[segments[i].startIdx+1];
				float radius = center.z;
				float sAngle = data.x;
				float eAngle = data.y;
				bool clock = data.z > 0.f;
				if(clock)
					std::swap(sAngle, eAngle);
				float diff = eAngle - sAngle;
				for(int i=0; i<rez; i++) {
					float angle = (i/(float)rez) * diff + sAngle;
					response.push_back(center + poPoint(cosf(angle)*radius, sinf(angle), 0.f));
				}
				break;
			}
			case PO_PATH_SEGMENT_QUADTO: {
				poPoint start = response.back(); 
				poPoint end = points[segments[i].startIdx];
				poPoint control = points[segments[i].startIdx+1];
				for(int i=0; i<rez; i++) {
					float t = i/(float)rez;
					float invT = 1.f - t;
					response.push_back(invT*invT*start + 2*invT*t*control + t*t*end);
				}
				break;
			}
			case PO_PATH_SEGMENT_CUBETO: {
				poPoint start = response.back();
				poPoint end = points[segments[i].startIdx];
				poPoint control1 = points[segments[i].startIdx+1];
				poPoint control2 = points[segments[i].startIdx+2];
				for(int i=0; i<rez; i++) {
					float t = i/(float)rez;
					float invT = 1.f - t;
					response.push_back(invT*invT*invT*start + 3*invT*invT*t*control1 + 3*invT*t*t*control2 + t*t*t*end);
				}
				break;
			}
			case PO_PATH_SEGMENT_CLOSE:
				bail = true;
				break;
		}
		
		if(bail)
			break;
	}
	
	return response;
}

