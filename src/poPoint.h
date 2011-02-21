/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

class poPoint
{
public:
	float x, y, z;
	
	poPoint();
	poPoint(float x, float y, float z=0.f);
	
	poPoint &set(float x, float y, float z);
	poPoint &setPolar(float deg, float mag);
	
	bool operator==(poPoint rhs);
	bool almostEqual(poPoint rhs, float range=0.01f);
	void operator+=(poPoint rhs);
	void operator-=(poPoint rhs);
	void operator*=(float scalar);
	void operator/=(float scalar);
	
	poPoint &normalize();
	
	float length() const;
	float lengthSquared() const;
	float dot(poPoint rhs) const;
	float dotSquared(poPoint rhs) const;
	poPoint cross(poPoint rhs) const;

	poPoint normal2D() const;
	poPoint rotate2D(float angle) const;
};

poPoint operator+(poPoint A, poPoint B);	// add
poPoint operator-(poPoint A, poPoint B);	// subtract
poPoint operator-(poPoint A);					// negate
poPoint operator*(poPoint A, float Scalar);		// multiply by scalar
poPoint operator*(float S, poPoint A);
poPoint operator*(poPoint A, poPoint B);
poPoint operator/(poPoint A, float Scalar);		// divide by scalar

poPoint normalize(poPoint pt);