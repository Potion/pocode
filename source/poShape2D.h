/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "poSimpleDrawing.h"

struct poVertex {
	
	enum Attribute {
		ATTRIB_POINT = 0,
		ATTRIB_COLOR = 1,
		ATTRIB_TEX_COORD = 2,
		ATTRIB_NORMAL = 4
	};
	
	poVertex(poPoint pt=poPoint(0,0), poPoint texc=poPoint(0,0), poColor col=poColor(1,1,1,1), poPoint norm=poPoint())
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
	
	enum StrokeCapProperty {
		STROKE_CAP_BUTT = 0,
		STROKE_CAP_ROUND = 1,
		STROKE_CAP_SQUARE = 2
	};
	enum StrokeJoinProperty {
		STROKE_JOIN_MITRE = 0,
		STROKE_JOIN_BEVEL = 1,
		STROKE_JOIN_ROUND = 2
	};
	
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

	// properties
	bool isFillEnabled() const;
	void enableFill(bool b);
	
	bool isStrokeEnabled() const;
	void enableStroke(bool b);
	
	int strokeWidth() const;
	void strokeWidth(int w);
	
	poColor fillColor() const;
	void fillColor(poColor c);
	
	poColor strokeColor() const;
	void strokeColor(poColor c);
	
	GLenum fillDrawStyle() const;
	void fillDrawStyle(GLenum e);

	bool isAttributeEnabled(poVertex::Attribute a) const;
	void enableAttribute(poVertex::Attribute a);
	
	StrokeCapProperty capStyle() const;
	void capStyle(StrokeCapProperty p);
	
	StrokeJoinProperty joinStyle() const;
	void joinStyle(StrokeJoinProperty p);
	
private:
	void generateStroke();
	
	std::vector<poVertex> vertices;
	std::vector<poPoint> stroke;

	bool enable_fill;
	bool enable_stroke;
	int stroke_width;
	poColor fill_color;
	poColor stroke_color;
	GLenum fill_draw_style;
	int enabled_attributes;
	StrokeCapProperty cap;
	StrokeJoinProperty join;
};

struct LineSeg { 
	poPoint p1, p2, p3, p4;
	LineSeg()
	{}
	LineSeg(poPoint ul, poPoint ll, poPoint ur, poPoint lr)
	:	p1(ul)
	,	p2(ll)
	,	p3(ur)
	,	p4(lr)
	{}
};

struct Ray {
	poPoint origin, dir;
	Ray(poPoint o, poPoint d)
	:	origin(o)
	,	dir(normalize(d))
	{}
};

// returns quads for a given line segment
LineSeg extrudeLineSegment(poPoint p1, poPoint p2, float width);
// cheap 'matrix' determinant
float determinant(poPoint row1, poPoint row2, poPoint row3);
// angle ABC
float angleBetweenPoints(poPoint a, poPoint b, poPoint c);
// angle ABC but with extruded line segments
float angleBetweenSegments(LineSeg seg1, LineSeg seg2);
// returns false if parallel, p1 = intersection or p1 and p2 = closest point on each
bool rayIntersection(Ray r1, Ray r2, poPoint *p1, poPoint *p2);
// returns angle between segments, out-vars are top/bottom intersections
float combineExtrudedLineSegments(LineSeg seg1, LineSeg seg2, poPoint *top, poPoint *bottom);
