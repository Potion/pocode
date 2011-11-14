//
//  poPath.cpp
//  pathTest
//
//  Created by Joshua Fisher on 11/12/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poPath.h"

poSubPath& poSubPath::start(poPoint startPoint) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_MOVETO,0,1));
	points.clear();
	points.push_back(startPoint);
	return *this;
}
poSubPath& poSubPath::lineTo(poPoint endPoint) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_LINETO,points.size(),1));
	points.push_back(endPoint);
	return *this;
}
poSubPath& poSubPath::arcTo(poPoint center, float startAngle, float endAngle, bool clockwise) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_ARCTO,points.size(),2));
	points.push_back(center);
	points.push_back(poPoint(startAngle, endAngle, clockwise));
	return *this;
}
poSubPath& poSubPath::curveTo(poPoint endPoint, poPoint control) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_CUBETO,points.size(),2));
	points.push_back(endPoint);
	points.push_back(control);
	return *this;
}
poSubPath& poSubPath::curveTo(poPoint endPoint, poPoint control1, poPoint control2) {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_QUADTO,points.size(),3));
	points.push_back(endPoint);
	points.push_back(control1);
	points.push_back(control2);
	return *this;
}
poSubPath& poSubPath::close() {
	segments.push_back(segment_storage(PO_PATH_SEGMENT_CLOSE,points.size(),0));
	return *this;
}
uint poSubPath::getNumSegments() const {
	return segments.size();
}
poPathSegmentType poSubPath::getSegmentType(uint idx) const {
	return segments[idx].type;
}
bool poSubPath::isSegmentType(uint idx, poPathSegmentType type) const {
	return segments[idx].type == type;
}

void poSubPath::modifyLineTo(uint idx, poPoint point) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_LINETO)) {
		points[segments[idx].startIdx] = point;
	}
}
void poSubPath::modifyArcTo(uint idx, poPoint center, float startAngle, float endAngle, bool clockwise) {
	segment_storage &storage = segments[idx];
	
	if(storage.count != 2) {
	}
	points[segments[idx].startIdx] = center;
	points[segments[idx].startIdx+1] = poPoint(startAngle, endAngle, clockwise);
}
void poSubPath::modifyQuadTo(uint idx, poPoint point, poPoint control) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_QUADTO)) {
		
	}
}
void poSubPath::modifyCubeTo(uint idx, poPoint point, poPoint control1, poPoint control2) {
	if(isSegmentType(idx, PO_PATH_SEGMENT_CUBETO)) {
		
	}
}
void poSubPath::removeSegment(uint idx) {
	segment_storage &storage =  segments[idx];

	std::vector<poPoint>::iterator iter = points.begin()+storage.startIdx;
	short count = storage.count;
	points.erase(iter, iter+count);
	
	segments.erase(segments.begin()+idx);
	for(int i=idx; i<segments.size(); i++) {
		segments[i].startIdx -= count;
	}
}
