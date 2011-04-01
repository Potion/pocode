/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "poSimpleDrawing.h"

struct poVertex {
	
	enum Attribute {
		POINT = 0,
		COLOR = 1,
		TEX_COORD = 2,
		NORMAL = 4
	};
	
	poVertex(poPoint pt=poPoint(), poColor col=poColor(1,1,1,1), poPoint norm=poPoint(), poPoint texc=poPoint(0,0))
	: point(pt), color(col), normal(norm), texCoord(texc)
	{}
	
	poPoint point;
	poColor color;
	poPoint texCoord;
	poPoint normal;
};


class poShape2D
:	public poObject
{
public:
	poShape2D();

	virtual void draw();

	void addPoint(poPoint p);
	void addVertex(poVertex v);
	void addPoints(const std::vector<poPoint> &points);
	void addVertices(const std::vector<poVertex> &vertices);
	void curveTo(poPoint pt, poPoint control, int resolution=10);
	void curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution=10);
	
	void setPoints(const std::vector<poPoint> &points);
	void setPoints(const std::vector<poVertex> &vertices);
	void clearPoints();

	size_t numPoints() const;
	poPoint &getPoint(int idx);
	poVertex &getVertex(int idx);
	
	virtual void setAlignment(poAlignment align);
	virtual poRect calculateBounds(bool include_children=false);

	bool enableFill;
	int strokeWidth;

	poColor fillColor;
	poColor strokeColor;
	
	GLenum fillDrawStyle;
	// bitmask of the attributes you actually want to use
	poVertex::Attribute enabledAttributes;
	
private:
	std::vector<poVertex> vertices;
};

