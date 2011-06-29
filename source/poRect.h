/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poPoint.h"

class poRect 
{
public:
	poPoint size, origin;
	
	poRect();
	poRect(float x, float y, float w, float h);
	poRect(const poPoint &origin, const poPoint &size);

	float width() const;
	float height() const;
	float area() const;
	poPoint center() const;
	
	poRect &set(float x, float y, float w, float h);
	poRect &set(poPoint origin, poPoint size);

	poRect &include(float x, float y);
	poRect &include(const poPoint &pt);
	poRect &include(const poRect &rect);
	poRect &scale(float scalar);
	// scale around a point
	poRect &scale(float scalar, const poPoint &pt);
	poRect &inset(poPoint p);
	
	bool contains(float x, float y) const;
	bool contains(const poPoint &pt) const;
	
	poPoint topLeft() const;
	poPoint bottomLeft() const;
	poPoint topRight() const;
	poPoint bottomRight() const;
	
	poPoint remap(poRect from, poPoint p);
	
	std::string toString() const;
};

std::ostream &operator<<(std::ostream &o, const poRect &r);
std::istream &operator>>(std::istream &i, poRect &r);
