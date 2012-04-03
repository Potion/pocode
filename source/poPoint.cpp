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

#include "poMath.h"
#include "poPoint.h"

#include <sstream>

poPoint::poPoint()
:	x(0.f), y(0.f), z(0.f)
{}

poPoint::poPoint(const float *f)
:	x(f[0]), y(f[1]), z(f[2])
{}

poPoint::poPoint(float x, float y, float z)
:	x(x), y(y), z(z)
{}

poPoint &poPoint::set(poPoint p) {
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
	return *this;
}

poPoint &poPoint::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

poPoint &poPoint::setPolar(float deg, float mag) {
	x = cos_deg(deg) * mag;
	y = sin_deg(deg) * mag;
	return *this;
}

bool poPoint::fromString(std::string const& str) {
	int count = sscanf(str.c_str(), "point(%f,%f,%f)", &x, &y, &z);
	if(count == 2)
		z = 0.f;
	else if(count < 2) {
		x = 0;
		y = 0;
		z = 0;
		return false;
	}
	return true;
}

std::string poPoint::toString() const {
	std::stringstream ss;
	ss << "point(" << x << "," << y << "," << z << ")";
	return ss.str();
}

bool poPoint::operator==(poPoint rhs) {
	return compare(*this, rhs);
}

void poPoint::operator+=(poPoint rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

void poPoint::operator-=(poPoint rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

void poPoint::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void poPoint::operator/=(float scalar) {
    if(scalar == 0) return;
    
	if(x != 0) x /= scalar;
	if(y != 0) y /= scalar;
	if(z != 0) z /= scalar;
}

poPoint &poPoint::normalize() {
	float len = getLength();
    if(len == 0) return *this;
    
	if(x != 0) x /= len;
	if(y != 0) y /= len;
	if(z != 0) z /= len;
	return *this;
}

float poPoint::getLength() const {
	return sqrtf(x*x + y*y + z*z);
}

float poPoint::getLengthSquared() const {
	return x*x + y*y + z*z;
}

float poPoint::getDist(poPoint pt) {
    return poDist(x, y, z, pt.x, pt.y, pt.z);
}

float poPoint::getDot(poPoint rhs) const {
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

poPoint poPoint::getCross(poPoint rhs) const {
	return poPoint(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
}

poPoint poPoint::getNormal2D() const {
	return poPoint(y, -x);
}

poPoint poPoint::getRotate2D(float deg) const {
	poPoint Xv(cos_deg(deg), sin_deg(deg));
	poPoint Yv(-sin_deg(deg), cos_deg(deg));
	return poPoint(Xv * x + Yv * y);
}

poPoint operator+(poPoint A, poPoint B) {
	return poPoint(A.x+B.x, A.y+B.y, A.z+B.z);
}

poPoint operator-(poPoint A, poPoint B) {
	return poPoint(A.x-B.x, A.y-B.y, A.z-B.z);
}

poPoint operator-(poPoint A) {
	return poPoint(-A.x, -A.y, -A.z);
}

poPoint operator*(poPoint A, float Scalar) {
	return poPoint(A.x*Scalar, A.y*Scalar, A.z*Scalar);
}

poPoint operator*(float S, poPoint A) {
	return operator*(A,S);
}

poPoint operator*(poPoint A, poPoint B) {
	return poPoint(A.x*B.x, A.y*B.y, A.z*B.z);
}

poPoint operator/(poPoint A, float Scalar) {
	return poPoint(A.x/Scalar, A.y/Scalar, A.z/Scalar);
}

poPoint normalize(poPoint pt) {
	float len = pt.getLength();
	return poPoint(pt.x/len, pt.y/len, pt.z/len);
}

bool compare(poPoint a, poPoint b, float range) {
	return (compare(a.x, b.x, range) &&
			compare(a.y, b.y, range) &&
			compare(a.z, b.z, range));
}

std::ostream &operator<<(std::ostream &o, const poPoint &p) {
	o << p.toString();
	return o;
}
