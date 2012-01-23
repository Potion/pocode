/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once


#include "poPoint.h"

#include <vector>

// CLASS NOTES
//
// poRect is a light-weight object used to specify bounding boxes and other rectangular forms
// throughout pocode.
//
// poRect is not a poObject and hence cannot be added to the scene graph. See poRectShape.
// 
// 

class poRect {
public:
	
    // CONSTRUCTORS
	poRect();
	poRect(float x, float y, float w, float h);
	poRect(const poPoint &origin, const poPoint &size);

    // RECT SET METHODS
    poRect&         set(float x, float y, float w, float h);
	poRect&         set(poPoint pos, poPoint size);
    
    poRect&         setPosition(float x, float y);
    poRect&         setPosition(poPoint pos);
    poRect&         setSize(float w, float h);
    poRect&         setSize(poPoint size);
    
    // RECT PROPERTIES
	poPoint         getPosition() const;
	poPoint         getSize() const;
	float           getArea() const;
	poPoint         getCenter() const;
	float           getAspect() const;
	
    // RECT CORNERS
    poPoint         getTopLeft() const;
	poPoint         getBottomLeft() const;
	poPoint         getTopRight() const;
	poPoint         getBottomRight() const;
	std::vector<poPoint> getCorners() const;

    // RECT EXPANSION
	poRect&         include(float x, float y);
	poRect&         include(const poPoint &pt);
	poRect&         include(const poRect &rect);
    
    // RECT SCALING
	poRect&         scale(float scalar);
	// scale around a point
	poRect&         scale(float scalar, const poPoint &pt);
	poRect&         inset(poPoint p);
	poRect&			expand(poPoint p);
	
    // RECT QUERYING
	bool            contains(float x, float y) const;
	bool            contains(const poPoint &pt) const;

	poPoint         remap(poRect from, poPoint p);
	
    // RECT TO STRING
	std::string     toString() const;
	bool            fromString(std::string const& str);
    
    float           x,y,width,height;
};

std::ostream &operator<<(std::ostream &o, const poRect &r);
