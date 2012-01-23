/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once


// CLASS NOTES
//
// poPoint is a three dimensional vector, composed of three floats X,Y and Z.
// poPoint is one of the most commonly used classes in pocode.
//
// poPoints are used to specify position and scale of objects. They are also used to represent
// vectors in space, as in the axis of rotation. poPoints also form the contours in poShape2D.
//
// poPoint overloads all common mathematical operators, so one may write A = B + C,
// where A,B and C are all poPoints.
//

#include <string>
#include <ostream>

class poPoint {
public:
    // CONSTRUCTORS
	poPoint();
	poPoint(const float *f);
	poPoint(float x, float y, float z=0.f);
	
    // SET POINT
	poPoint&        set(poPoint p);
	poPoint&        set(float x, float y, float z);
	poPoint&        setPolar(float deg, float mag);
    
    // SET POINT with STRING
    // Format is "point(x,y)" or "point(x,y,z)"
	bool			fromString(std::string const& str);
	std::string     toString() const;
	
    // POINT (VECTOR) OPERATIONS
	poPoint&        normalize();
	float           getLength() const;
	float           getLengthSquared() const;
    float           getDist(poPoint pt);
	float           getDot(poPoint rhs) const;
	poPoint         getCross(poPoint rhs) const;
	poPoint         getNormal2D() const;
	poPoint         getRotate2D(float angle) const;
    
    // OVERLOADED OPERATORS
    // For example, where A and B are both poPoints, and S is a scalar
    bool            operator==(poPoint rhs);    // A == B
	void            operator+=(poPoint rhs);    // A += B
	void            operator-=(poPoint rhs);    // A -= B
	void            operator*=(float scalar);   // A *= S
	void            operator/=(float scalar);   // A /= S
    
    // X, Y and Z
    float           x, y, z;
};

// OVERLOADED OPERATOR
// For example, where A and B are both poPoints, and S is a scalar
poPoint operator+(poPoint A, poPoint B);	// A = B + C

poPoint operator-(poPoint A, poPoint B);	// A = B - C
poPoint operator-(poPoint A);				// A = -B

poPoint operator*(poPoint A, float Scalar);	// A = B * S
poPoint operator*(float S, poPoint A);      // A = S * B
poPoint operator*(poPoint A, poPoint B);    // A = B * C (pair-wise multiply, same as dot product)

poPoint operator/(poPoint A, float Scalar);	// A = B / S

poPoint         normalize(poPoint pt);
bool            compare(poPoint a, poPoint b, float range=1.0e-6f);

std::ostream &operator<<(std::ostream &o, const poPoint &p);

