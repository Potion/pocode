//
//  poPath.h
//  pathTest
//
//  Created by Joshua Fisher on 11/12/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poRect.h"

enum poPathSegmentType {
	PO_PATH_SEGMENT_MOVETO,
	PO_PATH_SEGMENT_LINETO,
	PO_PATH_SEGMENT_ARCTO,
	PO_PATH_SEGMENT_QUADTO,
	PO_PATH_SEGMENT_CUBETO,
	PO_PATH_SEGMENT_CLOSE
};

class poPath {
public:
	poPath& start(poPoint startPoint);
	poPath& lineTo(poPoint endPoint);
	poPath& arcTo(poPoint center, float startAngle, float endAngle, bool clockwise);
	poPath& curveTo(poPoint endPoint, poPoint control);
	poPath& curveTo(poPoint endPoint, poPoint control1, poPoint control2);
	poPath& close();
	
	uint getNumSegments() const;
	poPathSegmentType getSegmentType(uint idx) const;
	bool isSegmentType(uint idx, poPathSegmentType type) const;
	void modifyLineTo(uint idx, poPoint point);
	void modifyArcTo(uint idx, poPoint center, float startAngle, float endAngle, bool clockwise);
	void modifyQuadTo(uint idx, poPoint point, poPoint control);
	void modifyCubeTo(uint idx, poPoint point, poPoint control1, poPoint control2);
	void removeSegment(uint idx);
	
	bool isEmpty() const;
	bool isOpen() const;

	std::vector<poPoint> generatePoints() const;

private:
	struct segment_storage {
		short startIdx, count;
		poPathSegmentType type;
		segment_storage(poPathSegmentType t, short sIdx, short count) 
		: type(t)
		, startIdx(sIdx)
		, count(count) 
		{}
	};
	std::vector<segment_storage> segments;
	std::vector<poPoint> points;
};

