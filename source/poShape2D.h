/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "poSimpleDrawing.h"

struct VertexPack {
	VertexPack(poPoint pt=poPoint(), poColor col=poColor(1,1,1,1), poPoint texc=poPoint(0,0))
	: point(pt), color(col), texCoord(texc)
	{}
	
	poPoint point;
	poColor color;
	poPoint texCoord;
};
typedef std::vector<VertexPack> VertexPackVec;

std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution=10);
std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution=10);

class poShape2D
:	public poObject
{
public:
	poShape2D();

	virtual void draw();

	void moveTo(poPoint pt);
	void moveTo(float x, float y);
	void lineTo(poPoint pt);
	void lineTo(float x, float y);
	void quadTo(poPoint pt, poPoint control, int resolution=10);
	void cubeTo(poPoint pt, poPoint control1, poPoint control2, int resolution=10);
	void close();

	virtual void setAlignment(poAlignment align);
	virtual poRect calculateBounds(bool include_children=false);

	int numPoints() const;
	poPoint &getPoint(int idx);
	VertexPack &getPack(int idx);
	
//	void placeTexture(ci::gl::Texture tex, float x=0, float y=0, float scale=1.f, float rot=0.f);
//	void placeTexture(ci::gl::Texture tex, poTextureScaleOption scaleOption=PO_TEX_NO_FIT);
	
	poColor fillColor;
	int strokeWidth;
	poColor strokeColor;
	GLenum drawStyle;
	
private:
	VertexPackVec vertices;
//	ci::gl::Texture texture;
};

