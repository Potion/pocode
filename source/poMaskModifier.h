//
//  poMaskModifier.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poTexture.h"
#include "poObjectModifier.h"

class poShape2D;

class poImageMask 
:	public poObjectModifier
{
public:
	poImageMask(poTexture *tex);
	poImageMask(const std::string &str);
	virtual ~poImageMask();
	
protected:	
	virtual void doSetUp( poObject* obj );
	virtual void doSetDown( poObject* obj );
	
private:
	poTexture *texture;
};

class poGeometryMask
:	public poObjectModifier
{
public:
	// you are responsible for the object lifetime of this shape
	poGeometryMask(poShape2D *shape);

protected:
	virtual void doSetUp( poObject* obj );
	virtual void doSetDown( poObject* obj );
	
private:
	poShape2D *shape;
};