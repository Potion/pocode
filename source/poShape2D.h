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

    // manage shape points
	poShape2D &addPoint(poPoint p);
    poShape2D &addPoint( float x, float y );
	poShape2D &addPoints(const std::vector<poPoint> &points);
	poShape2D &curveTo(poPoint pt, poPoint control, int resolution=10);
	poShape2D &curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution=10);
	
	void setPoints(const std::vector<poPoint> &points);
	poShape2D &clearPoints();

	size_t              numPoints() const;
	poPoint				getPoint(int idx);
    bool                setPoint(int idx, poPoint p );
	const std::vector<poPoint> &getPoints();
	poPoint				getTexCoord(int idx, uint unit=0);
	poColor				getColor(int idx); 
	
    // alignment and bounds
	virtual void		alignment(poAlignment align);
	// this was a weird one: http://stackoverflow.com/questions/888235/overriding-a-bases-overloaded-function-in-c/888313#888313
	using poObject::alignment;
	
	virtual poRect      calculateBounds(bool include_children=false);
	
    // texture management, will copy the texture
	poShape2D&			placeTexture(poTexture *tex, uint unit=0);
	poShape2D&			placeTexture(poTexture *tex, poTextureFitOption fit, uint unit=0);

	poShape2D&			placeImage(poImage *img, uint unit=0);
	poShape2D&			placeImage(poImage *img, poTextureFitOption fit, uint unit=0);
	
	poShape2D&			generateStroke(int strokeWidth, StrokeJoinProperty join=STROKE_JOIN_MITRE, StrokeCapProperty cap=STROKE_CAP_BUTT);
    
	// get and set shape properties
	poShape2D&          enableFill(bool b);
    bool                isFillEnabled() const;
    
	poShape2D&          enableStroke(bool b);
    bool                isStrokeEnabled() const;
    
    int                 strokeWidth() const;
    
	poShape2D&          fillColor(poColor c);
    poShape2D&          fillColor( float r, float g, float b, float a=1.0 );
    poColor             fillColor() const;
    
	poShape2D&          strokeColor(poColor c);
    poShape2D&          strokeColor( float r, float g, float b, float a=1.0 );
    poColor             strokeColor() const;
    
	poShape2D&          fillDrawStyle(GLenum e);
    GLenum              fillDrawStyle() const;
    
    poShape2D&          useSimpleStroke( bool b );
    bool                isSimpleStrokeEnabled() const;
    
    StrokeCapProperty   capStyle() const;
    
    StrokeJoinProperty  joinStyle() const;
    
	poShape2D&          closed(bool b);
    bool                isClosed() const;
	
	poShape2D&			drawBounds(bool b);
	bool				drawBounds() const;
    
    GLenum              textureCombineFunction(uint unit=0) const;
	poShape2D&          textureCombineFunction(GLenum func, uint unit=0);
	poTexture const*	texture(uint unit=0) const;
	
	bool				alphaTestTextures() const;
	poShape2D&			alphaTestTextures(bool b);
    
    poShape2D&          enableAttribute(VertexAttribute a);
	poShape2D&          disableAttribute(VertexAttribute a);
    bool                isAttributeEnabled(VertexAttribute a) const;

    // localize will convert global to local first
	// otherwise, point is assumed to be local
	virtual bool        pointInside(poPoint point, bool localize=false);

	poTween<poColor>	fill_color_tween;
	
protected:
	virtual void		updateAllTweens();
    
private:    
	std::vector<poPoint>    points;
	std::vector<poPoint>    tex_coords[MAX_TEXTURE_UNITS];
	std::vector<poColor>    colors;
	std::vector<poPoint>    stroke;

	bool                    enable_fill;
	bool                    enable_stroke;
	int                     stroke_width;
	poColor                 fill_color;
	poColor                 stroke_color;
	GLenum                  fill_draw_style;
	int                     enabled_attributes;
    bool                    use_simple_stroke;
	StrokeCapProperty       cap;
	StrokeJoinProperty      join;
	bool                    closed_;
	bool					draw_bounds;
	bool					alpha_test_textures;
    
	std::vector<GLenum>         tex_combo_func;
	std::vector<poTexture*>     textures;
	poResourceStore				resources;
};


