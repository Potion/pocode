//
//  poCamera.h
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poColor.h"


class poCamera2D : public poObjectModifier
{
public:
    poCamera2D();
	poCamera2D(poColor clear, bool reset=true);
	poCamera2D(poColor clear, float near, float far ,bool reset=true);
    
    virtual void setUp( poObject* obj );
    virtual void setDown( poObject* obj );

	bool clearsBackground() const;
	void clearsBackground(bool b);
	
	poColor backgroundColor() const;
	void backgroundColor(poColor color);
	void backgroundColor(float r, float g, float b, float a=1.f);
    
private:
    bool		clears_background;
    poColor		background_color;
	bool		reset;
    float       near;
    float       far;
};