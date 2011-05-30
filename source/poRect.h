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

	void include(float x, float y);
	void include(const poPoint &pt);
	void include(const poRect &rect);
	void scale(float scalar);
	// scale around a point
	void scale(float scalar, const poPoint &pt);
	
	bool contains(float x, float y) const;
	bool contains(const poPoint &pt) const;
	
	poPoint topLeft() const;
	poPoint bottomLeft() const;
	poPoint topRight() const;
	poPoint bottomRight() const;
	
	std::string toString() const;
};


std::ostream &operator<<(std::ostream &o, const poRect &r);
std::istream &operator>>(std::istream &i, poRect &r);
