/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "SimpleDrawing.h"
#include "poTexture.h"

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
	
	poShape2D &setPoints(const std::vector<poPoint> &points);
	poShape2D &clearPoints();

	size_t              numPoints() const;
	poPoint				getPoint(int idx);
    bool                setPoint(int idx, poPoint p );
	const std::vector<poPoint> &getPoints();
	
    // texture management, will copy the texture
	poShape2D&			placeTexture(poTexture *tex);
	poShape2D&			placeTexture(poTexture *tex, poTextureFitOption fit);
	poShape2D&			placeTexture(poTexture *tex, poTextureFitOption fit, poAlignment align);

	poShape2D&			transformTexture(poPoint pt, poPoint scale, float rotate);

    poStrokeCapProperty   capStyle() const;
    poStrokeJoinProperty  joinStyle() const;
	int                 strokeWidth() const;
	poShape2D&			generateStroke(int strokeWidth, poStrokePlacementProperty place=PO_STROKE_PLACE_CENTER, poStrokeJoinProperty join=PO_STROKE_JOIN_MITRE, poStrokeCapProperty cap=PO_STROKE_CAP_BUTT);

	// localize will convert global to local first
	// otherwise, point is assumed to be local
	virtual bool        pointInside(poPoint point, bool localize=false);

	GLenum				fillDrawStyle;
    poColor				fillColor;
	poColor				strokeColor;
	bool				fillEnabled;
	bool				strokeEnabled;
	bool				useSimpleStroke;
	bool				closed;
	bool				drawBounds;
	bool				alphaTestTexture;
	
	poTween<poColor>	fill_color_tween;

	virtual void		stopAllTweens(bool recurse=false);

protected:
	virtual void		updateAllTweens();
    
private:    
	std::vector<poPoint>    points;
	std::vector<poPoint>    tex_coords;
	std::vector<poPoint>    stroke;

	poTexture*				texture;

	poStrokeCapProperty     cap;
	poStrokeJoinProperty    join;
	int						stroke_width;
};

std::vector<poShape2D*> createShapesFromSVGfile(const fs::path &svg);
