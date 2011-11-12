//
//  poPath.cpp
//  potionCode
//
//  Created by Joshua Fisher on 11/11/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poPath.h"
#include "poSimpleDrawing.h"

poPathSegment& poPathSegment::setStart(poPoint p) {
	type = PO_PATH_SEG_START;
	point = p;
}
poPathSegment& poPathSegment::setLineTo(poPoint p) {
	type = PO_PATH_SEG_LINE_TO;
	point = p;
}
poPathSegment& poPathSegment::setQuadTo(poPoint p, poPoint c) {
	type = PO_PATH_SEG_QUAD_TO;
	point = p;
	control1 = c;
}
poPathSegment& poPathSegment::setCubeTo(poPoint p, poPoint c1, poPoint c2) {
	type = PO_PATH_SEG_CUBE_TO;
	point = p;
	control1 = c1;
	control2 = c2;
}
poPathSegment& poPathSegment::setClose() {
	type = PO_PATH_SEG_CLOSE;
}

void poPath::start(poPoint p) {
	segments.clear();
	segments.push_back(poPathSegment().setStart(p));
}
void poPath::start(float x, float y) {
	start(poPoint(x,y,0));
}
void poPath::lineTo(poPoint p) {
	segments.push_back(poPathSegment().setLineTo(p));
}
void poPath::lineTo(float x, float y) {
	lineTo(poPoint(x,y,0));
}
void poPath::quadTo(poPoint p, poPoint control) {
	segments.push_back(poPathSegment().setQuadTo(p,control));
}
void poPath::quadTo(float x, float y, float cx, float cy) {
	quadTo(poPoint(x,y,0), poPoint(cx,cy,0));
}
void poPath::cubeTo(poPoint p, poPoint control1, poPoint control2) {
	segments.push_back(poPathSegment().setCubeTo(p, control1, control2));
}
void poPath::cubeTo(float x, float y, float c1x, float c1y, float c2x, float c2y) {
	cubeTo(poPoint(x,y,0), poPoint(c1x,c1y,0), poPoint(c2x,c2y,0));
}
void poPath::close() {
	segments.push_back(poPathSegment().setClose());
}
std::vector<poPoint> poPath::generatePoints(int curveResolution) const {
	std::vector<poPoint> response;

	for(int i=0; i<segments.size(); ++i) {
		poPathSegment const& seg = segments[i];
		
		switch(seg.type) {
			case PO_PATH_SEG_START:
			case PO_PATH_SEG_LINE_TO:
				response.push_back(seg.point);
				break;
			case PO_PATH_SEG_QUAD_TO: {
				poPoint p0 = response.back();
				poPoint p1 = seg.control1;
				poPoint p2 = seg.point;
				for(int i=0; i<curveResolution; i++) {
					float t = (i+1) / (float)curveResolution;
					response.push_back( ((1-t)*(1-t)*p0) + (2*(1-t)*t*p1) + (t*t*p2) );
				}
				response.push_back(p2);
				break;
			}
			case PO_PATH_SEG_CUBE_TO: {
				poPoint p0 = response.back();
				poPoint p1 = seg.control1;
				poPoint p2 = seg.control2;
				poPoint p3 = seg.point;
				for(int i=0; i<curveResolution; i++) {
					float t = (i+i) / (float)curveResolution;
					response.push_back( ((1-t)*(1-t)*(1-t)*p0) + (3*(1-t)*(1-t)*t*p1) + (3*(1-t)*t*t*p2) + (t*t*t*p3) );
				}
				response.push_back(p3);
				break;
			}
			case PO_PATH_SEG_CLOSE:
				response.push_back(segments[0].point);
				break;
		}
	}
	
	return response;
}

poPathSegmentIter poPath::begin() {
	return segments.begin();
}
poPathSegmentIter poPath::end() {
	return segments.end();
}


void drawPath(poPath const &path) {
	std::vector<poPoint> points = path.generatePoints();
	po::drawPoints(points, GL_LINE_STRIP);
}

