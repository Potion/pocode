/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poRect.h"
#include "poMath.h"

#include <sstream>

using namespace std;

poRect::poRect() 
:	x(0)
,	y(0)
,   width(0)
,   height(0)
{}

poRect::poRect(float x, float y, float w, float h)
:	x(x)
,	y(y)
,   width(w)
,   height(h)
{}

poRect::poRect(const poPoint &p1, const poPoint &p2)
:	x(p1.x)
,   y(p1.y)
,	width(p2.x)
,   height(p2.y)
{}

poRect& poRect::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    return *this;
}

poRect& poRect::setPosition(poPoint pos) {
    x = pos.x;
    y = pos.y;
    return *this;
}

poPoint poRect::getPosition() const {
	return poPoint(x,y);
}

poRect& poRect::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    return *this;
}

poRect& poRect::setSize(poPoint size) {
    width = size.x;
    height = size.y;
    return *this;
}

poPoint poRect::getSize() const {
	return poPoint(width, height);
}

float poRect::getArea() const {
	return width * height;
}

poPoint poRect::getCenter() const {
	return poPoint(width/2.f + x, height/2.f+y);
}

float poRect::getAspect() const {
	return width / height;
}

poRect &poRect::set(float x, float y, float w, float h) {
    setPosition(x,y);
    setSize(w,h);
	return *this;
}

poRect &poRect::set(poPoint pos, poPoint size) {
	setPosition(pos);
	setSize(size);
	return *this;
}

poRect &poRect::include(float x, float y) {
    poPoint maxPoint = getPosition() + getSize();
    
    poPoint pos = getPosition();
    
    maxPoint.x  = std::max(x, maxPoint.x);
	maxPoint.y  = std::max(y, maxPoint.y);
    pos.x       = std::min(x, pos.x);
	pos.y       = std::min(y, pos.y);
    width       = maxPoint.x-pos.x;
    height      = maxPoint.y-pos.y;
    
    setPosition(pos);
	return *this;
}

poRect &poRect::include(const poPoint &pt) {
	return include(pt.x, pt.y);
}

poRect &poRect::include(const poRect &rect) {
	include(rect.getPosition());
	return include(rect.getPosition() + rect.getSize());
}

poRect &poRect::scale(float scalar) {
    poPoint size = getSize();
	size *= scalar;
    setSize(size);
	return *this;
}

poRect &poRect::scale(float scalar, const poPoint &pt) {
    setPosition((getPosition() - pt) * scalar + pt);
    setSize(getSize() * scalar);
	return *this;
}

poRect &poRect::inset(poPoint p) {
	setPosition(getPosition() + p);
    setSize(getSize() - (p *2));
	return *this;
}

poRect &poRect::expand(poPoint p) {
	setPosition(getPosition() - p);
	setSize(getSize() + (p*2));
	return *this;
}

poRect poRect::getFlipped() const {
	return poRect(x, y+height, width, -height);
}

bool poRect::contains(float x, float y) const {
	return	x >= this->x && 
			x <= (this->x+width) && 
			y >= this->y && 
			y <= (this->y+height);
}

bool poRect::contains(const poPoint &pt) const {
	return contains(pt.x, pt.y);
}

poPoint poRect::getTopLeft() const {
	return getPosition();
}

poPoint poRect::getBottomLeft() const {
	return poPoint(x, y + height);
}

poPoint poRect::getTopRight() const {
	return poPoint(x+width, y);
}

poPoint poRect::getBottomRight() const {
	return getPosition() + getSize();
}

std::vector<poPoint> poRect::getCorners() const {
	std::vector<poPoint> response;
	response.push_back(getTopLeft());
	response.push_back(getTopRight());
	response.push_back(getBottomRight());
	response.push_back(getBottomLeft());
	return response;
}

std::string poRect::toString() const {
	std::stringstream ss;
	ss << "rect(" << x << "," << y << "," << width << "," << height << ")";
	return ss.str();
}

bool poRect::fromString(const std::string &str) {
	float x, y, w, h;
	int wrote = sscanf(str.c_str(), "rect(%f,%f,%f,%f)", &x, &y, &w, &h);
	if(wrote < 4) {
		set(0,0,0,0);
		return false;
	}
	
	set(x,y,w,h);
	return true;
}

poPoint poRect::remap(poRect from, poPoint p) {
	poPoint from_ll = from.getTopLeft();
	poPoint from_ur = from.getBottomRight();
	
	poPoint to_ll = getTopLeft();
	poPoint to_ur = getBottomRight();

	poPoint pt;
	pt.x = poMapf(from_ll.x, from_ur.x, p.x, to_ll.x, to_ur.x);
	pt.y = poMapf(from_ll.y, from_ur.y, p.y, to_ll.y, to_ur.y);
	return pt;
}

std::ostream &operator<<(std::ostream &o, const poRect &r) {
	o << r.toString();
	return o;
}
