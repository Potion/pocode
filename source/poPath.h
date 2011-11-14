//
//  poPath.h
//  potionCode
//
//  Created by Joshua Fisher on 11/11/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poPoint.h"

enum poSegmentType {
	PO_PATH_SEG_START,
	PO_PATH_SEG_LINE_TO,
	PO_PATH_SEG_QUAD_TO,
	PO_PATH_SEG_CUBE_TO,
	PO_PATH_SEG_CLOSE
};

class poPathSegment {
public:
	poPathSegment& setStart(poPoint p);
	poPathSegment& setLineTo(poPoint p);
	poPathSegment& setQuadTo(poPoint p, poPoint c);
	poPathSegment& setCubeTo(poPoint p, poPoint c1, poPoint c2);
	poPathSegment& setClose();
	
	poSegmentType type;
	poPoint point, control1, control2;
};

typedef std::vector<poPathSegment>::iterator poPathSegmentIter;

// a single line with no breaks
class poPath {
public:
	// will drop all the points and start over
	void start(poPoint p);
	void start(float x, float y);
	// append a line
	void lineTo(poPoint p);
	void lineTo(float x, float y);
	// append curves
	void quadTo(poPoint p, poPoint control);
	void quadTo(float x, float y, float cx, float cy);
	void cubeTo(poPoint p, poPoint control1, poPoint control2);
	void cubeTo(float x, float y, float c1x, float c1y, float c2x, float c2y);
	// mark the path as closed
	void close();

	std::vector<poPoint> generatePoints(int curveResolution=25) const;
	
	poPathSegmentIter begin();
	poPathSegmentIter end();
	
	std::vector<poPathSegment> segments;
};

void drawPath(poPath const &path);

