/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poPoint.h"

class poRect 
{
public:
	float left, right, top, bottom;
	
	poRect();
	poRect(float l, float r, float t, float b);
	poRect(const poPoint &pt, float w, float h);
	poRect(const poPoint &p1, const poPoint &p2);
	
	poRect &set(float l, float r, float t, float b);
	
	float area() const;
	float width() const;
	float height() const;
	poPoint center() const;

	void include(float x, float y);
	void include(const poPoint &pt);
	void include(const poRect &rect);
	void scale(float scalar);
	// scale around a point
	void scale(float scalar, const poPoint &pt);
	void cannonicalize();
	
	bool contains(float x, float y) const;
	bool contains(const poPoint &pt) const;
	
	poPoint topLeft() const;
	poPoint bottomLeft() const;
	poPoint topRight() const;
	poPoint bottomRight() const;
};