//
//  poShapeBasics2D.h
//  poFramework4
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//
#pragma once

#include "poShape2D.h"
#include "poTexture.h"

class poRectShape : public poShape2D
{
public:
	poRectShape();
	// rounded rect shape
	poRectShape( float width, float height, float rad=0.f );
	// place an already-made texture
	poRectShape( poTexture* tex, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT );
	// make a texture from the specified url and place it
	poRectShape( const std::string &str, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT);
	// make a shape of a certain size and put the texture in place inside it
	poRectShape( float width, float height, poTexture* tex, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT );
    
    void reshape( float width, float height, float rad=0.f );
    
protected:
    void construct( float _width, float _height, float rad );
    
    float width, height, radius;
};

class poOvalShape : public poShape2D
{
public:
    poOvalShape();
    poOvalShape( float _width, float _height, int numPoints );
    
    void    reshape( float _width, float _height, int numPoints );
    
protected:
    void    construct( float _width, float _height, int numPoints );
    
    float   width, height;
};


class poLineShape : public poShape2D
{
public:
    poLineShape();
    poLineShape( poPoint A, poPoint B );
    
    void    reshape( poPoint A, poPoint B );
    
protected:
    void    construct( poPoint A, poPoint B );
};


