/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "poSimpleDrawing.h"
#include "poTexture.h"

enum VertexAttribute {
	ATTRIB_POINT = 0,
	ATTRIB_COLOR = 1,
	ATTRIB_TEX_COORD = 2,
	ATTRIB_NORMAL = 4
};

struct poVertex {
	poVertex(poPoint pt=poPoint(0,0), poPoint texc=poPoint(0,0), poColor col=poColor(1,1,1,1), poPoint norm=poPoint())
	: point(pt), color(col), normal(norm), texCoord(texc)
	{}
	
	poPoint point;
	poColor color;
	poPoint texCoord;
	poPoint normal;
};

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
	
	void placeTexture(poTexture tex);
	void generateStroke();

	// properties
	bool isFillEnabled() const;
	bool isStrokeEnabled() const;
	int strokeWidth() const;
	poColor fillColor() const;
	poColor strokeColor() const;
	GLenum fillDrawStyle() const;
	bool isAttributeEnabled(VertexAttribute a) const;
	StrokeCapProperty capStyle() const;
	StrokeJoinProperty joinStyle() const;
	bool isClosed() const;

	void enableFill(bool b);
	void enableStroke(bool b);
	void strokeWidth(int w);
	void fillColor(poColor c);
	void strokeColor(poColor c);
	void fillDrawStyle(GLenum e);
	void enableAttribute(VertexAttribute a);
	void disableAttribute(VertexAttribute a);
	void capStyle(StrokeCapProperty p);
	void joinStyle(StrokeJoinProperty p);
	void closed(bool b);

private:
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
	poTexture texture;
	bool closed_;
};