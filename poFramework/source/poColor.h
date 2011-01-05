/*
 *  poColor.h
 *  jXGLapp
 *
 *  Created by Jared Schiffman on 2/18/06.
 *  Copyright 2006 Potion Design LLC. All rights reserved.
 *
 */

#include <vector>

#ifndef PO_COLOR_H
#define PO_COLOR_H

class poColor
{
public:
	poColor();
	poColor( float r, float g, float b );
	poColor( float r, float g, float b, float a );
	poColor( float gray );
	void operator=(const poColor& c);
	
	void	apply();
	void	apply( float Alpha );
	
	poColor&	set( float r, float g, float b );
	poColor&	set255( float r, float g, float b );
	poColor&	set( float r, float g, float b, float a );
	poColor&	setHSV( float h, float s, float v ) ;
	poColor&	setCMYK( float c, float m, float y, float k );
	
	bool	operator==(poColor& B);
	void	operator+=(poColor& B);
	void	operator-=(poColor& B);
	void	operator*=(float F);
	void	operator/=(float F);
	
	float	R, G, B, A;
	
	static const poColor white;
	static const poColor black;
	static const poColor grey;
	static const poColor red;
	static const poColor yellow;
	static const poColor orange;
	static const poColor blue;
	static const poColor green;
};


poColor operator+(poColor& A, poColor& B);
poColor operator-(poColor& A, poColor& B);
poColor operator*(poColor& A, float Scalar);
poColor operator/(poColor& A, float Scalar);

typedef std::vector<poColor> poColorVector;

#endif PO_COLOR_H