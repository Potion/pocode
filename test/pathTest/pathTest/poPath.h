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

class poSubPath {
public:
	poSubPath& start(poPoint startPoint);
	poSubPath& lineTo(poPoint endPoint);
	poSubPath& arcTo(poPoint center, float startAngle, float endAngle, bool clockwise);
	poSubPath& curveTo(poPoint endPoint, poPoint control);
	poSubPath& curveTo(poPoint endPoint, poPoint control1, poPoint control2);
	poSubPath& close();
	
	uint getNumSegments() const;
	poPathSegmentType getSegmentType(uint idx) const;
	bool isSegmentType(uint idx, poPathSegmentType type) const;
	void modifyLineTo(uint idx, poPoint point);
	void modifyArcTo(uint idx, poPoint center, float startAngle, float endAngle, bool clockwise);
	void modifyQuadTo(uint idx, poPoint point, poPoint control);
	void modifyCubeTo(uint idx, poPoint point, poPoint control1, poPoint control2);
	void removeSegment(uint idx);
	
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

class poPath {
public:
	poPath &moveTo(poPoint startPoint);
	poPath &lineTo(poPoint endPoint);
	poPath &curveTo(poPoint endPoint, poPoint control);
	poPath &curveTo(poPoint endPoint, poPoint control1, poPoint control2);
	poPath &arcTo(poPoint arcCenter, float arcRadius, float startAngle, float endAngle, bool clockwise);
	poPath &close();
	
	poPath &addRect(poRect r);
	poPath &addElipseInRect(poRect r);
	
	size_t getNumSubPaths() const;
	void addSubpath(poSubPath const& subPath);
	void insertSubPath(uint idx, poSubPath const& subPath);
	void removeSubPath(uint idx);
	void setSubPath(uint idx, poSubPath const& subPath);
	poSubPath &getSubPath(uint idx) const;
	poSubPath &getCurrentSubPath() const;
	
private:
	std::vector<poSubPath> subPaths;
};


