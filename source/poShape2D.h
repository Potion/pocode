/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poObject.h"
#include "SimpleDrawing.h"
#include "poTexture.h"


// CLASS NOTES
//
// poShape2D derives directly from poObject and is the parent class of all 2D graphics objects
// in potionCode, including poRectShape, poOvalShape and poLineShape (see "poShapeBasics2D.h").
//
// All poShape2D objects and poShape2D subclasses:
//
// + Have all the properties and methods of poObject.
// + Have a list of "points" that form the contour of the shape.
// + Have fillColor and strokeColor properties
// + Have fillEnabled and strokeEnabled properties.
// + Have a fillDrawStyle property that determines how OpenGL uses the points (GL_TRIANGLE_STRIP, GL_POLYGON, etc)
// + Have a generateStroke method for generating a high-quality thick stroke with multiple capping options.
// + Have a placeTexture and and transformTexture methods for assigning a poTexture to the shape
// + Have a pointInside method for testing if a point is inside the shape.


class poShape2D : public poObject
{
public:
	poShape2D();
	virtual poObject*		copy();

    // DRAWING
    // The draw() method is called automatically if a shape is added to the scene graph.
    // You should not need to call the draw() method yourself.
	virtual void            draw();

    // SHAPE POINTS
    // poShape2D maintains a list of points that define the contour the shape.
    // Points can be added one by one, or can be added as vector of points.
    // The "curveTo" method generates a curve formed by adding many points.
	poShape2D&				addPoint(poPoint p);
    poShape2D&				addPoint( float x, float y );
	poShape2D&				addPoints(const std::vector<poPoint> &points);
	poShape2D&				curveTo(poPoint pt, poPoint control, int resolution=10);
	poShape2D&				curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution=10);
	
	poShape2D&				setPoints(const std::vector<poPoint> &points);
	poShape2D&				clearPoints();

	size_t                  getNumPoints() const;
	poPoint                 getPoint(int idx);
    bool                    setPoint(int idx, poPoint p );
	const std::vector<poPoint> &getPoints();
	
    // SHAPE TEXTURE
    // A poShape2D can be assigned a poTexture.
    // By default, the texture will be placed within the shape at actual size. This means that
    // the texture image may extend beyond the bounds of the shape or may not be large enough
    // to fill the shape. Use the poTextureFitOption's to determine how a texture is placed
    // into the shape. These poTextureFitOption's are listed in poEnums.h. Once a texture is placed,
    // you can use transform the placement, rotation and scale of the texture.
	poShape2D&              placeTexture(poTexture tex);
	poShape2D&              placeTexture(poTexture tex, poTextureFitOption fit);
	poShape2D&              placeTexture(poTexture tex, poTextureFitOption fit, poAlignment align);
	poShape2D&              transformTexture(poPoint pt, poPoint scale, float rotate);
    
    // HIGH QUALITY SHAPE STROKE
    // By default, poShape2D's use a simple OpenGL stroke. Unfortunately, when the stroke width
    // is greater than 3 pixels, the OpenGL stroke appear broken at every corner. As a solution,
    // poShape2D includes a "generateStroke" method that creates a very high quality stroke.
    // This stroke is actually drawn as a very thin filled shape. These high quality strokes
    // scale with the shape, unlike simple strokes which do not scale.
	poShape2D&              generateStroke(int strokeWidth, poStrokePlacementProperty place=PO_STROKE_PLACE_CENTER, 
                                           poStrokeJoinProperty join=PO_STROKE_JOIN_MITRE, poStrokeCapProperty cap=PO_STROKE_CAP_BUTT);
    poStrokeCapProperty     capStyle() const;
    poStrokeJoinProperty    joinStyle() const;
    
	void                    setStrokeWidth(int width);
	int                     getStrokeWidth() const;

    // POINT INSIDE
    // pointInside assumes points are in window-native coordinates (0,0 is in the upper left).
    // The localize option will convert the point from global to local coordinates before
    // performing the test.
    virtual bool            pointInside(poPoint point, bool localize=false);

    // TWEEN MANAGEMENT
    virtual void            stopAllTweens(bool recurse=false);
    
    // SHAPE SERIALIZAATION
	virtual void            read(poXMLNode node);
	virtual void            write(poXMLNode &node);
    
    // SHAPE PROPERTIES (DIRECTLY ACCCESSIBLE)
	GLenum                  fillDrawStyle;
    poColor                 fillColor;
	poColor                 strokeColor;
	bool                    fillEnabled;
	bool                    strokeEnabled;
	bool                    useSimpleStroke;
	bool                    closed;
    
    // SHAPE SPECIFIC TWEEN
    // In addition to the five tweens in poObject, poShape2D has a special tween for the fillColor.
	poTween<poColor>        fillColorTween;


protected:
	virtual void            updateAllTweens();
	void					clone(poShape2D *shp);
    
private:
    // SHAPE PROPERTIES (PRIVATE)
	std::vector<poPoint>    points;
	std::vector<poPoint>    texCoords;
	std::vector<poPoint>    stroke;

	poTexture				texture;

	poStrokeCapProperty     cap;
	poStrokeJoinProperty    join;
	int						strokeWidth;
};


// SHAPE GENERATOR FROM SVG FILE
std::vector<poShape2D*> createShapesFromSVGfile(const fs::path &svg);


