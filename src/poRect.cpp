/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poRect.h"
#include "poMath.h"

poRect::poRect() 
:	left(0.f), right(0.f), top(0.f), bottom(0.f)
{}

poRect::poRect(float l, float r, float t, float b)
:	left(l), right(r), top(t), bottom(b)
{
	cannonicalize();
}

poRect::poRect(const poPoint &pt, float w, float h) 
:	left(pt.x), right(pt.x+w), top(pt.y), bottom(pt.y+h)
{
	cannonicalize();
}

poRect::poRect(const poPoint &p1, const poPoint &p2)
:	left(p1.x), right(p2.x), top(p1.y), bottom(p2.y)
{
	cannonicalize();
}

poRect &poRect::set(float l, float r, float t, float b) {
	left = l;
	right = r;
	top = t;
	bottom = b;
	cannonicalize();
	return *this;
}

float poRect::area() const {
	return width()*height();
}

float poRect::width() const {
	return right - left;
}

float poRect::height() const {
	return bottom - top;
}

poPoint poRect::center() const {
	return poPoint(width()/2.f+left, height()/2.f+top);
}

void poRect::include(float x, float y) {
	left = std::min(x, left);
	right = std::max(x, right);
	top = std::min(y, top);
	bottom = std::max(y, bottom);
}

void poRect::include(const poPoint &pt) {
	include(pt.x, pt.y);
}

void poRect::include(const poRect &rect) {
	left = std::min(rect.left, left);
	right = std::max(rect.right, right);
	top = std::min(rect.top, top);
	bottom = std::max(rect.bottom, bottom);
}

void poRect::scale(float scalar) {
	left *= scalar;
	right *= scalar;
	top *= scalar;
	bottom *= scalar;
}

void poRect::scale(float scalar, const poPoint &pt) {
	left = (left - pt.x) * scalar;
	right = (right - pt.x) * scalar;
	top = (top - pt.y) * scalar;
	bottom = (bottom - pt.y) * scalar;
}

void poRect::cannonicalize() {
	if(top > bottom)
		std::swap(top,bottom);
	if(left > right)
		std::swap(left,right);
}

bool poRect::contains(float x, float y) const {
	return x >= left && x <= right && y >= top && y <= bottom;
}

bool poRect::contains(const poPoint &pt) const {
	return contains(pt.x, pt.y);
}

poPoint poRect::topLeft() const {
	return poPoint(left, top);
}

poPoint poRect::bottomLeft() const {
	return poPoint(left, bottom);
}

poPoint poRect::topRight() const {
	return poPoint(right, top);
}

poPoint poRect::bottomRight() const {
	return poPoint(right, bottom);
}
