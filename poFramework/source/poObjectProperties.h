/*
 *  poObjectProperties.h
 *  poFramework2
 *
 *  Created by Joshua Fisher on 2/1/08.
 *  Copyright 2008 Potion Design, LLC. All rights reserved.
 *
 */

#ifndef _PO_OBJECT_PROPERTIES_H_
#define _PO_OBJECT_PROPERTIES_H_

#include "poCommon.h"

class	poObjectProperties
{
public:
	poObjectProperties()
	: position(0,0,0), alpha(1.0f), scale(1,1,1), rotation(0.0f), rotationAxis(0,0,1), visible(true) {}

	poObjectProperties(const poPoint& p, float a, const poPoint& s)
	: position(p), alpha(a), scale(s), rotation(0.0f), rotationAxis(0,0,1), visible(true) {}

	poObjectProperties(const poPoint& p, float a, const poPoint& s, float r, const poPoint& rA, bool v )
	: position(p), alpha(a), scale(s), rotation(r), rotationAxis(rA), visible(v) {}


	poPoint position;
	float	alpha;
	poPoint scale;

	float	rotation;
	poPoint rotationAxis;

	bool	visible;
};

typedef std::vector<poObjectProperties> poObjectPropertyVector;


#endif