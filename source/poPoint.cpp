/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poMath.h"
#include "poPoint.h"

poPoint::poPoint()
:	x(0.f), y(0.f), z(0.f)
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

bool poPoint::set(const std::string &str) {
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
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

poPoint &poPoint::normalize() {
	float len = length();
	x /= len;
	y /= len;
	z /= len;
	return *this;
}

float poPoint::length() const {
	return sqrtf(x*x + y*y + z*z);
}

float poPoint::lengthSquared() const {
	return x*x + y*y + z*z;
}

float poPoint::dot(poPoint rhs) const {
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

poPoint poPoint::cross(poPoint rhs) const {
	return poPoint(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
}

poPoint poPoint::normal2D() const {
	return poPoint(y, -x);
}

poPoint poPoint::rotate2D(float deg) const {
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
	float len = pt.length();
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
