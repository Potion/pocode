//
//  poShapeBasics2D.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poShapeBasics2D.h"



#pragma mark poRectShape

poRectShape::poRectShape()
{
    construct( 100,100 );
}

poRectShape::poRectShape( float _width, float _height )
{
    construct( _width, _height );
}

poRectShape::poRectShape( poImage *img) {
	construct(img->width(), img->height());
	poTexture tex(img);
	placeTexture(&tex);
}

poRectShape::poRectShape( poTexture* tex )
{
    construct( tex->width(), tex->height() );
    placeTexture( tex );
}

poRectShape::poRectShape( const std::string &str) {
	poTexture tex(str);
	construct(tex.width(), tex.height());
	placeTexture(&tex);
}

void    poRectShape::reshape( float w, float h )
{
	clearPoints();
	construct(w, h);
	
	calculateBounds();
	if(isStrokeEnabled())
		generateStroke(strokeWidth());
}

poRectShape::poRectShape( float w, float h, poTexture* tex, poTextureFitOption fit ) {
	construct(w, h);
	placeTexture(tex, fit);
}

void poRectShape::construct( float w, float h )
{
    width = w;
    height = h;
    
    addPoint( poPoint(0,0) );
    addPoint( poPoint(width,0) );
    addPoint( poPoint(width,height) );
    addPoint( poPoint(0,height) );
}


#pragma mark poRoundRectShape

poRoundRectShape::poRoundRectShape(float w, float h, float corner_radius) {
	reshape(w,h,corner_radius);
}

void poRoundRectShape::reshape(float w, float h) {
	reshape(w, h, rad);
}

void poRoundRectShape::reshape(float w, float h, float corner_radius) {
	width = w;
	height = h;
	rad = corner_radius;

	clearPoints();
	construct();
}

void poRoundRectShape::construct() {
	addPoint(poPoint(0, rad));
	addPoints(quadTo(getPoints().back(), poPoint(rad, 0), poPoint(0, 0), 10));

	addPoint(poPoint(width-rad, 0));
	addPoints(quadTo(getPoints().back(), poPoint(width,rad), poPoint(width, 0), 10));

	
	addPoint(poPoint(width, height-rad));
	addPoints(quadTo(getPoints().back(), poPoint(width-rad, height), poPoint(width, height), 10));

	addPoint(poPoint(rad, height));
	addPoints(quadTo(getPoints().back(), poPoint(0, height-rad), poPoint(0, height), 10));
	
	closed(true);
}

#pragma mark poOvalShape

poOvalShape::poOvalShape()
{
    construct( 100,100,30 );
}

poOvalShape::poOvalShape( float _width, float _height, int nPoints )
{
    construct( _width, _height, nPoints );
}

void    poOvalShape::reshape( float _width, float _height, int nPoints )
{
    // if the number of points is the same as before, just use the existing point
    if ( nPoints == numPoints() )
    {
        width = _width;
        height = _height;
        
        float dA = M_2PI / (float) nPoints;
        for( int i=0; i<nPoints; i++ )
            setPoint( i, poPoint(_width/2.0*cos(dA*i), _height/2.0*sin(dA*i), 0) );
    }
    else    // if not, clear the list and make new points
    {
        clearPoints();
        construct( _width, _height, nPoints );
    }
	
	calculateBounds();
	if(isStrokeEnabled())
		generateStroke(strokeWidth());
}

void poOvalShape::construct( float _width, float _height, int nPoints )
{
    width = _width;
    height = _height;
    
    float dA = M_2PI / (float) nPoints;
    for( int i=0; i<nPoints; i++ )
    {
        addPoint( _width/2.0*cos(dA*i), _height/2.0*sin(dA*i) );
    }
}

// ============== poOvalShape ================================

poLineShape::poLineShape()
{
    construct( poPoint(0,0,0), poPoint(0,100,0) );
}

poLineShape::poLineShape( poPoint A, poPoint B )
{
    construct( A,B );
}

void    poLineShape::reshape( poPoint A, poPoint B )
{
    setPoint( 0, A );
    setPoint( 1, B );
}

void    poLineShape::construct( poPoint A, poPoint B )
{
    addPoint( A );
    addPoint( B );
    
    closed( false );
    enableFill( false );
	generateStroke(1,STROKE_JOIN_MITRE,STROKE_CAP_ROUND);
}


