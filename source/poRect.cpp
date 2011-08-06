/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poRect.h"
#include "poMath.h"

using namespace std;

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

float poRect::aspect() const {
	return width() / height();
}

poRect &poRect::set(float x, float y, float w, float h) {
	origin.set(x,y,0);
	size.set(w,h,0);
	return *this;
}

poRect &poRect::set(poPoint o, poPoint s) {
	origin = o;
	size = s;
	return *this;
}

bool poRect::set(const std::string &str) {
	float x, y, w, h;
	int wrote = sscanf(str.c_str(), "rect(%f,%f,%f,%f)", &x, &y, &w, &h);
	if(wrote < 4) {
		set(0,0,0,0);
		return false;
	}
	
	set(x,y,w,h);
	return true;
}

poRect &poRect::include(float x, float y) {
    poPoint maxPoint = origin + size;
    
    maxPoint.x = std::max(x, maxPoint.x);
	maxPoint.y = std::max(y, maxPoint.y);
    origin.x = std::min(x, origin.x);
	origin.y = std::min(y, origin.y);
    size.x = maxPoint.x-origin.x;
    size.y = maxPoint.y-origin.y;
	return *this;
}

poRect &poRect::include(const poPoint &pt) {
	return include(pt.x, pt.y);
}

poRect &poRect::include(const poRect &rect) {
	include(rect.origin);
	return include(rect.origin + rect.size);
}

poRect &poRect::scale(float scalar) {
	size *= scalar;
	return *this;
}

poRect &poRect::scale(float scalar, const poPoint &pt) {
	origin = (origin - pt) * scalar + pt;
	size *= scalar;
	return *this;
}

poRect &poRect::inset(poPoint p) {
	origin += p;
	size -= p * 2;
	return *this;
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

std::vector<poPoint> poRect::corners() const {
	std::vector<poPoint> response;
	response.push_back(topLeft());
	response.push_back(topRight());
	response.push_back(bottomRight());
	response.push_back(bottomLeft());
	return response;
}

std::string poRect::toString() const {
	return (boost::format("%.2f %.2f %.2f %.2f") % origin.x % origin.y % size.x % size.y).str();
}

poPoint poRect::remap(poRect from, poPoint p) {
	poPoint from_ll = from.topLeft();
	poPoint from_ur = from.bottomRight();
	
	poPoint to_ll = topLeft();
	poPoint to_ur = bottomRight();

	poPoint pt;
	pt.x = poMapf(from_ll.x, from_ur.x, p.x, to_ll.x, to_ur.x);
	pt.y = poMapf(from_ll.y, from_ur.y, p.y, to_ll.y, to_ur.y);
	return pt;
}

std::ostream &operator<<(std::ostream &o, const poRect &r) {
	o << r.toString();
	return o;
}

std::istream &operator>>(std::istream &i, poRect &r) {
	i >> r.origin >> r.size;
	return i;
}
