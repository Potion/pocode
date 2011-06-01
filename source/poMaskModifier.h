//
//  poMaskModifier.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poRect.h"
#include "poTexture.h"
#include "poObjectModifier.h"

class poImageMask 
:	public poObjectModifier
{
public:
	poImageMask(poTexture *tex);
	poImageMask(const std::string &str);
	virtual ~poImageMask();
	
	virtual void setUp( poObject* obj );
	virtual void setDown( poObject* obj );
	
private:
	poTexture *texture;
};

class poGeometryMask
:	public poObjectModifier
{
public:
	poGeometryMask(poRect r);

	virtual void setUp( poObject* obj );
	virtual void setDown( poObject* obj );
	
private:
	std::vector<poPoint> points;
};