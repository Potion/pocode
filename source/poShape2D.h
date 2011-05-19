/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "poSimpleDrawing.h"
#include "poTexture.h"

#define MAX_TEXTURE_UNITS 8

class poShape2D
:	public poObject
{
public:
	poShape2D();

	virtual void draw();

	void                addPoint(poPoint p);
	void                addPoints(const std::vector<poPoint> &points);
	void                curveTo(poPoint pt, poPoint control, int resolution=10);
	void                curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution=10);
        
	void                setPoints(const std::vector<poPoint> &points);
	void                clearPoints();

	size_t              numPoints() const;
	poPoint&            getPoint(int idx);
	poPoint&            getTexCoord(int idx, uint unit=0);
	poColor&            getColor(int idx); 
	
	virtual void        setAlignment(poAlignment align);
	virtual poRect      calculateBounds(bool include_children=false);
	
	void                placeTexture(poTexture *tex, uint unit=0);
	void                generateStroke();

	// properties
	bool                isFillEnabled() const;
	bool                isStrokeEnabled() const;
	int                 strokeWidth() const;
	poColor             fillColor() const;
	poColor             strokeColor() const;
	GLenum              fillDrawStyle() const;
	bool                isAttributeEnabled(VertexAttribute a) const;
	StrokeCapProperty   capStyle() const;
	StrokeJoinProperty  joinStyle() const;
	bool                isClosed() const;
	GLenum              textureCombineFunction(uint unit=0) const;

	poShape2D&          enableFill(bool b);
	poShape2D&          enableStroke(bool b);
	poShape2D&          strokeWidth(int w);
	poShape2D&          fillColor(poColor c);
	poShape2D&          strokeColor(poColor c);
	poShape2D&          fillDrawStyle(GLenum e);
	poShape2D&          capStyle(StrokeCapProperty p);
	poShape2D&          joinStyle(StrokeJoinProperty p);
	poShape2D&          closed(bool b);
	poShape2D&          textureCombineFunction(GLenum func, uint unit=0);
    poShape2D&          enableAttribute(VertexAttribute a);
	poShape2D&          disableAttribute(VertexAttribute a);
    
private:
	std::vector<poPoint> points;
	std::vector<poPoint> tex_coords[MAX_TEXTURE_UNITS];
	std::vector<poColor> colors;
	std::vector<poPoint> stroke;

	bool                enable_fill;
	bool                enable_stroke;
	int                 stroke_width;
	poColor             fill_color;
	poColor             stroke_color;
	GLenum              fill_draw_style;
	int                 enabled_attributes;
	StrokeCapProperty   cap;
	StrokeJoinProperty  join;
	bool                closed_;
    
	std::vector<GLenum>         tex_combo_func;
	std::vector<poTexture*>     textures;
	poResourceStore*            resources;
};


