/*
 *  poPoint.cpp
 *  jXGLapp
 *
 *  Created by Jared Schiffman on 2/18/06.
 *  Copyright 2006 Potion Design LLC. All rights reserved.
 *
 */

#include "poPoint.h"
#include "poCommon.h"

poPoint poPoint::zero = poPoint(0,0,0);

poPoint::poPoint(void) {
	x = y = z = 0.0;
	w = t = 0.0;
	mark = 0;
}

poPoint::poPoint(float x, float y)
{
	this->x = x;
	this->y = y;
	z = w = t = mark = 0;
}

poPoint::poPoint(float x, float y, float z) 
{
	this->x = x;
	this->y = y;
	this->z = z;
	w = t = mark = 0;
}

poPoint& poPoint::set(float newX, float newY, float newZ) {
	x = newX;
	y = newY;
	z = newZ;
	return *this;
}

poPoint& poPoint::set(float newX, float newY)
{
	x = newX;
	y = newY;
	z = 0;
	return *this;
}

poPoint& poPoint::setPolar( float deg, float mag )
{
	x = dcos(deg) * mag;
	y = dsin(deg) * mag;
	z = 0;
	return *this;
}

poPoint& poPoint::set(poPoint& B) {
	x = B.x;
	y = B.y;
	z = B.z;
	return *this;
}


bool poPoint::operator==(poPoint& B) {
	return (x==B.x && y==B.y && z==B.z);
}

void poPoint::operator+=(const poPoint& B) {
	set( x+B.x, y+B.y, z+B.z );
	w += B.w;
	t += B.t;
}

void poPoint::operator-=(const poPoint& B) {
	set( x-B.x, y-B.y, z-B.z );
	w -= B.w;
	t -= B.t;
}

void poPoint::operator*=(float F) {
	set( x*F, y*F, z*F );
	w *= F;
	t *= F;

}

void poPoint::operator/=(float F) {
	set( x/F, y/F, z/F );
	w /= F;
	t /= F;
}

bool poPoint::almostEqual(poPoint& B, float range)
{
	return (fabs(B.x-x) < range && fabs(B.y-y) < range && fabs(B.z-z) < range);
}

void poPoint::translate(const poPoint& t)
{
	x += t.x;
	y += t.y;
	z += t.z;
}

void poPoint::translate(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void poPoint::scale(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void poPoint::rotate2D(float deg)
{
	poPoint Xv( dcos(deg), dsin(deg), 0 );
	poPoint Yv( -dsin(deg), dcos(deg), 0 );
	poPoint tmp = Xv * x + Yv * y;
	x = tmp.x;
	y = tmp.y;
}

float poPoint::dot(poPoint &B) {
	float D;
	D = x*B.x + y*B.y + z*B.z;
	return D;
}

poPoint	poPoint::cross(poPoint &B) {
	poPoint C;
	C.set(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
	return C;
}

void poPoint::componentMult(poPoint &B) {
	set( x*B.x, y*B.y, z*B.z );
}

poPoint poPoint::normalize(poPoint &A) {
	float N;
	poPoint C;
	N = A.x*A.x + A.y*A.y + A.z*A.z;
	N = sqrt(N);
	C.set(A.x/N, A.y/N, A.z/N);
	return C;
}

poPoint& poPoint::normalize() {
	float N;
	N = x*x + y*y + z*z;		
	N = sqrt(N);
	if (N != 0) {
		x /= N; y /= N; z /= N;
	}
	return *this;
}

float poPoint::length() {
	return sqrt(x*x + y*y + z*z);
}

float poPoint::lengthSquared() {
	return x*x + y*y + z*z;
}


/* operators */

poPoint operator+(const poPoint &A, const poPoint &B) {
	poPoint C;
	C.set(A.x+B.x, A.y+B.y, A.z+B.z);
	C.w = A.w + B.w;
	C.t = A.t + B.t;
	return C;
}

poPoint operator-(const poPoint &A, const poPoint &B) {
	poPoint C;
	C.set(A.x-B.x, A.y-B.y, A.z-B.z);
	C.w = A.w - B.w;
	C.t = A.t - B.t;
	return C;
}

poPoint operator-(const poPoint &A) {
	poPoint C;
	C.set(-A.x, -A.y, -A.z);
	return C;
}

poPoint operator*(const poPoint &A, float M) {
	poPoint C;
	C.set(A.x*M, A.y*M, A.z*M);
	C.w = A.w * M;
	C.t = A.t * M;
	return C;
}

poPoint operator*(float M, const poPoint& A) {
	poPoint C;
	C.set(A.x*M, A.y*M, A.z*M);
	C.w = A.w * M;
	C.t = A.t * M;
	return C;
}

poPoint operator/(const poPoint &A, float M) {
	poPoint C;
	C.set(A.x/M, A.y/M, A.z/M);
	C.w = A.w / M;
	C.t = A.t / M;
	return C;
}

poPoint poPoint::get2Dnormal()
{
	poPoint ret = *this;
	ret.x = this->y;
	ret.y = -this->x;
	return ret;
}

