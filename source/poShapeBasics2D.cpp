//
//  poShapeBasics2D.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poShapeBasics2D.h"



// ============== poRectShape ================================

poRectShape::poRectShape()
{
    construct( 100,100 );
}

poRectShape::poRectShape( float _width, float _height )
{
    construct( _width, _height );
}

void poRectShape::construct( float _width, float _height )
{
    width = _width;
    height = _height;
    
    addPoint( poPoint(0,0) );
    addPoint( poPoint(width,0) );
    addPoint( poPoint(width,height) );
    addPoint( poPoint(0,height) );
}