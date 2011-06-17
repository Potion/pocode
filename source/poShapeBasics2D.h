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
    poRectShape( poTexture* tex );
	poRectShape( poImage *img);
	poRectShape( const std::string &str);
    poRectShape( float width, float height );
	poRectShape( float width, float height, poTexture* tex, poTextureFitOption fit=PO_TEX_FIT_NONE );
    
    void    reshape( float width, float height );
    
protected:
    void    construct( float _width, float _height );
    
    float   width, height;
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


