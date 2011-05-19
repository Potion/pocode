//
//  poShapeBasics2D.h
//  poFramework4
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#include "poShape2D.h"

class poRectShape : public poShape2D
{
public:
    poRectShape();
    poRectShape( float _width, float _height );
    
    
protected:
    void    construct( float _width, float _height );
    
    float   width, height;
};