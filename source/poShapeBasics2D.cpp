//
//  poShapeBasics2D.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poShapeBasics2D.h"
#include "Helpers.h"


#pragma mark poRectShape

poRectShape::poRectShape()
{
    construct( 100,100,0.f );
}

poRectShape::poRectShape( float _width, float _height, float rad )
{
    construct( _width, _height, rad );
}

poRectShape::poRectShape( poImage *img) {
	construct(img->width(), img->height(), 0.f);
	poTexture tex(img);
	placeTexture(&tex);
}

poRectShape::poRectShape( poTexture* tex )
{
    construct( tex->width(), tex->height(), 0.f );
    placeTexture( tex );
}

poRectShape::poRectShape( const std::string &str) {
	poTexture tex(str);
	construct(tex.width(), tex.height(), 0.f);
	placeTexture(&tex);
}

void    poRectShape::reshape( float w, float h, float rad )
{
	clearPoints();
	construct(w, h, rad);
	
	calculateBounds();
	if(isStrokeEnabled())
		generateStroke(strokeWidth());
}

poRectShape::poRectShape( float w, float h, poTexture* tex, poTextureFitOption fit ) {
	construct(w, h, 0);
	placeTexture(tex, fit);
}

void poRectShape::construct( float w, float h, float rad )
{
    width = w;
    height = h;
	radius = rad;
	
	if(rad > 0.f) {
		addPoints(roundedRect(width,height,rad));
	}
    else {
		addPoint( poPoint(0,0) );
		addPoint( poPoint(width,0) );
		addPoint( poPoint(width,height) );
		addPoint( poPoint(0,height) );
	}
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
	generateStroke(1,STROKE_PLACE_CENTER,STROKE_JOIN_MITRE,STROKE_CAP_ROUND);
}


