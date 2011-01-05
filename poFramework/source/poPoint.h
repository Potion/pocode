/*
 *  poPoint.h
 *  jXGLapp
 *
 *  Created by Jared Schiffman on 2/18/06.
 *  Copyright 2006 Potion Design LLC. All rights reserved.
 *
 */

#include <vector>
#include <list>

#ifndef PO_POINT_H
#define PO_POINT_H

class poPoint
{
public:
	float	x,y,z;
	float	w, t;
	// mark == 100 is for compound illustrator shapes
	char    mark;
	
	poPoint();
	poPoint(float x, float y);
	poPoint(float x, float y, float z);
	
	poPoint& set(float newX, float newY);
	poPoint& set(float newX, float newY, float newZ);
	poPoint& set(poPoint& B);
	poPoint& setPolar( float deg, float mag );
	
	bool	operator==(poPoint& B);
	void	operator+=(const poPoint& B);
	void	operator-=(const poPoint& B);
	void	operator*=(float F);
	void	operator/=(float F);
	bool	almostEqual(poPoint& B, float range=0.01f);

	void	translate(const poPoint& t);
	void	translate(float x, float y, float z=0.f);
	void	scale(float scalar);
	
	poPoint&	normalize();
	static poPoint normalize(poPoint &A);

	float		length();
	float		lengthSquared();

	void		componentMult(poPoint &B);

	float		dot(poPoint &B);
	
	poPoint		get2Dnormal();
	poPoint     cross(poPoint &B);
	
	void		rotate2D(float angle);
	void		rotate3D(float angle, poPoint& axis);
	
	static poPoint zero;
};


poPoint operator+(const poPoint &A, const poPoint &B);	// add
poPoint operator-(const poPoint &A, const poPoint &B);	// subtract
poPoint operator-(const poPoint &A);					// negate
poPoint operator*(const poPoint &A, float Scalar);		// multiply by scalar
poPoint operator/(const poPoint &A, float Scalar);		// divide by scalar
poPoint operator*(float S, const poPoint& A);

typedef std::vector<poPoint> poPointVector;
typedef std::list<poPoint> poPointList;

#endif

