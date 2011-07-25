//
//  poMaskModifier.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poShape2D.h"
#include "poObjectModifier.h"

class poGeometryMask : public poObjectModifier
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