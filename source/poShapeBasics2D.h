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
	poRectShape( const poTexture &tex );
	poRectShape( poImage *img);
	poRectShape( const std::string &str);
    poRectShape( float width, float height, float rad=0.f );
	poRectShape( float width, float height, poTexture* tex, poTextureFitOption fit=PO_TEX_FIT_NONE );
    
    void    reshape( float width, float height, float rad=0.f );
    
protected:
    void    construct( float _width, float _height, float rad );
    
    float   width, height, radius;
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


class poStarShape : public poShape2D
{
public:
    poStarShape();
    poStarShape( float _outerRadius, int numPoints, float depth );
    poStarShape( float _width, float _height, int numPoints, float depth );
    
    void reshape( float _outerRadius, int numPoints, float depth );
    void reshape( float _width, float _height, int numPoints, float depth );
    
protected:
    void construct( float _outerRadius, int numPoints, float depth );
    void construct( float _width, float _height, int numPoints, float depth );
    
    float width, height, depth;
    float outerRadius, innerRadius;
};
