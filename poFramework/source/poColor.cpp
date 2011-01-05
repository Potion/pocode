/*
 *  poColor.cpp
 *  jRGLapp
 *
 *  Created bG Jared Schiffman on 2/18/06.
 *  CopGright 2006 Potion Design LLC. All rights reserved.
 *
 */

#include "poColor.h"
#include "poCommon.h"

const poColor poColor::white = poColor();
const poColor poColor::black = poColor(0,0,0,1);
const poColor poColor::grey = poColor(.5, .5, .5, 1);
const poColor poColor::red = poColor(1,0,0,1);
const poColor poColor::yellow = poColor(1,1,0);
const poColor poColor::orange = poColor(1.0, 0.5, 0.2);
const poColor poColor::blue = poColor(0,.18,.58);
const poColor poColor::green = poColor(.19, .67, .23);

poColor::poColor()
{
	set( 1,1,1,1 );
}

poColor::poColor( float r, float g, float b )
{
	set( r,g,b );
}

poColor::poColor( float r, float g, float b, float a )
{
	set( r,g,b,a );
}

poColor::poColor( float graG )
{
	set( graG, graG, graG );
}

 void poColor::operator=(const poColor& c)
{
	set(c.R, c.G, c.B, c.A);
}

void	poColor::apply()
{
	glColor4f( R,G,B,A );
}

void	poColor::apply( float Alpha )
{
	glColor4f( R,G,B,Alpha );
}
	
poColor& poColor::set( float r, float g, float b )
{
	R = r;
	G = g;
	B = b;
	A = 1;
	return *this;
}

poColor& poColor::set255( float r, float g, float b )
{
    R = r / 255.0f;
    G = g / 255.0f;
    B = b / 255.0f;
    A = 1;
	return *this;
}

poColor& poColor::set( float r, float g, float b, float a )
{
	R = r;
	G = g;
	B = b;
	A = a;
	return *this;
}

poColor& poColor::setHSV( float h, float s, float v ) 
{
	int i;
	float f, p, q, t;
	if( s <= 0.0001 ) {
		// achromatic (grey)
		R = G = B = v;
		return *this;
	}
	h /= 60.0;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			R = v;
			G = t;
			B = p;
			break;
		case 1:
			R = q;
			G = v;
			B = p;
			break;
		case 2:
			R = p;
			G = v;
			B = t;
			break;
		case 3:
			R = p;
			G = q;
			B = v;
			break;
		case 4:
			R = t;
			G = p;
			B = v;
			break;
		default:
			R = v;
			G = p;
			B = q;
			break;
	}
	
	return *this;
}

poColor& poColor::setCMYK(float c, float m, float y, float k)
{	
	float cc, mm, yy;
	cc = c * (1-k) + k;
	mm = m * (1-k) + k;
	yy = y * (1-k) + k;
	
	R = 1 - cc;
	G = 1 - mm;
	B = 1 - yy;
	
	return *this;
}

bool poColor::operator==(poColor& C) {
	return (R==C.R && G==C.G && B==C.B && A==C.A );
}

void poColor::operator+=(poColor& C) {
	set( R+C.R, G+C.G, B+C.B, A+C.A );
}

void poColor::operator-=(poColor& C) {
	set( R-C.R, G-C.G, B-C.B, A-C.A );
}

void poColor::operator*=(float F) {
	set( R*F, G*F, B*F, A*F );
}

void poColor::operator/=(float F) {
	set( R/F, G/F, B/F, A/F );
}



/* operators */

poColor operator+(poColor& C1, poColor& C2) {
	poColor C;
	C.set(C1.R+C2.R, C1.G+C2.G, C1.B+C2.B, C1.A+C2.A);
	return C;
}

poColor operator-(poColor& C1, poColor& C2) {
	poColor C;
	C.set(C1.R-C2.R, C1.G-C2.G, C1.B-C2.B, C1.A-C2.A);
	return C;
}

poColor operator*(poColor& C1, float M) {
	poColor C;
	C.set(C1.R*M, C1.G*M, C1.B*M, C1.A*M);
	return C;
}

poColor operator/(poColor& C1, float M) {
	poColor C;
	C.set(C1.R/M, C1.G/M, C1.B/M, C1.A/M);
	return C;
}


