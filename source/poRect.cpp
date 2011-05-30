/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poRect.h"
#include "poMath.h"

poRect::poRect() 
:	origin(0,0)
,	size(0,0)
{}

poRect::poRect(float x, float y, float w, float h) {
	origin.set(x,y,0);
	size.set(w,h,0);
}

poRect::poRect(const poPoint &p1, const poPoint &p2)
:	origin(p1)
,	size(p2)
{}

float poRect::width() const {
	return size.x;
}

float poRect::height() const {
	return size.y;
}

float poRect::area() const {
	return width()*height();
}

poPoint poRect::center() const {
	return poPoint(width()/2.f+origin.x, height()/2.f+origin.y);
}

void poRect::include(float x, float y) {
	origin.x = std::min(x, origin.x);
	size.x = std::max(x, origin.x+size.x) - origin.x;
	origin.y = std::min(y, origin.y);
	size.y = std::max(y, origin.y+size.y) - origin.y;
}

void poRect::include(const poPoint &pt) {
	include(pt.x, pt.y);
}

void poRect::include(const poRect &rect) {
	include(rect.origin);
	include(rect.origin + rect.size);
}

void poRect::scale(float scalar) {
	size *= scalar;
}

void poRect::scale(float scalar, const poPoint &pt) {
	origin = (origin - pt) * scalar + pt;
	size *= scalar;
}

bool poRect::contains(float x, float y) const {
	return	x >= origin.x && 
			x <= (origin.x+size.x) && 
			y >= origin.y && 
			y <= (origin.y+size.y);
}

bool poRect::contains(const poPoint &pt) const {
	return contains(pt.x, pt.y);
}

poPoint poRect::topLeft() const {
	return origin;
}

poPoint poRect::bottomLeft() const {
	return poPoint(origin.x, origin.y+size.y);
}

poPoint poRect::topRight() const {
	return poPoint(origin.x+size.x, origin.y);
}

poPoint poRect::bottomRight() const {
	return origin + size;
}

std::string poRect::toString() const {
	return (boost::format("%.2f %.2f %.2f %.2f") % origin.x % origin.y % size.x % size.y).str();
}

std::ostream &operator<<(std::ostream &o, const poRect &r) {
	o << r.toString();
	return o;
}

std::istream &operator>>(std::istream &i, poRect &r) {
	i >> r.origin >> r.size;
	return i;
}
