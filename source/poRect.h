/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poPoint.h"

// CLASS NOTES
//
// poRect is a light-weight object used to specify bounding boxes and other rectangular forms
// throughout potionCode.
//
// poRect is not a poObject and hence cannot be added to the scene graph. See poRectShape.
// 
// 
//


class poRect 
{
public:
	
    // CONSTRUCTORS
	poRect();
	poRect(float x, float y, float w, float h);
	poRect(const poPoint &origin, const poPoint &size);

    // RECT SET METHODS
    poRect&         set(float x, float y, float w, float h);
	poRect&         set(poPoint origin, poPoint size);
	bool            set(const std::string &str);
    
    // RECT PROPERTIES
	float           width() const;
	float           height() const;
	float           area() const;
	poPoint         center() const;
	float           aspect() const;
	
    // RECT CORNERS
    poPoint         topLeft() const;
	poPoint         bottomLeft() const;
	poPoint         topRight() const;
	poPoint         bottomRight() const;
	std::vector<poPoint> corners() const;

    // RECT EXPANSION
	poRect&         include(float x, float y);
	poRect&         include(const poPoint &pt);
	poRect&         include(const poRect &rect);
    
    // RECT SCALING
	poRect&         scale(float scalar);
	// scale around a point
	poRect&         scale(float scalar, const poPoint &pt);
	poRect&         inset(poPoint p);
	
    // RECT QUERYING
	bool            contains(float x, float y) const;
	bool            contains(const poPoint &pt) const;

	poPoint         remap(poRect from, poPoint p);
	
    // RECT TO STRING
	std::string     toString() const;
    
    poPoint         size, origin;
};

std::ostream &operator<<(std::ostream &o, const poRect &r);
