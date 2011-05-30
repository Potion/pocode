/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poColor.h"
#include "poMath.h"

#define min3(a,b,c) a < b ? (a < c ? a : c) : (b < c ? b : c)
#define max3(a,b,c) a > b ? (a > c ? a : c) : (b > c ? b : c)


const poColor poColor::white = poColor(1,1,1);
const poColor poColor::black = poColor(0,0,0);
const poColor poColor::grey = poColor(.5, .5, .5);
const poColor poColor::red = poColor(1,0,0);
const poColor poColor::yellow = poColor(1,1,0);
const poColor poColor::orange = poColor(1.0, 0.5, 0.2);
const poColor poColor::blue = poColor(0,.18,.58);
const poColor poColor::green = poColor(.19, .67, .23);


poHSVColor rgba2hsv(poColor rgba) {
	float r = rgba.R * rgba.A;
	float g = rgba.G * rgba.A;
	float b = rgba.B * rgba.A;
	
	float min = min3(r,g,b);
	float max = max3(r,g,b);
	float range = max - min;
	float V = max;
	float S = 0.f;
	float H = 0.f;
	
	if(max != 0)
		S = range / max;
	
	if(S != 0) {
		if(rgba.R == max)
			H = (rgba.G - rgba.B) / range;
		else if(rgba.G == max)
			H = 2 + (rgba.B - rgba.R) / range;
		else
			H = 4 + (rgba.R - rgba.G) / range;
		
		H *= 60.f;
		if(H < 0.f)
			H += 360.f;
	}
	
	return poHSVColor(H,S,V);
}

poColor hsv2rgba(poHSVColor hsv) {
	float h6 = hsv.H / 60.0;
	
	int i = (int)floorf(h6);
	float f = h6 - i;
	float p = hsv.V * (1.0 - hsv.S);
	float q = hsv.V * (1.0 - (hsv.S * f));
	float t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));
	float v = hsv.V;
	
	poColor color;
	switch(i) {
		case 0:	color.set(v, t, p); break;
		case 1: color.set(q, v, p); break;
		case 2: color.set(p, v, t); break;
		case 3: color.set(p, q, v); break;
		case 4: color.set(t, p, v); break;
		case 5: color.set(v, p, q); break;
	}
	return color;
}

poColor::poColor()
:	R(0.f)
,   G(0.f)
,   B(0.f)
,   A(1.f)
{}

poColor::poColor(float r, float g, float b, float a)
:	R(r)
,   G(g)
,   B(b)
,   A(a)
{}

poColor::poColor(const poHSVColor &hsv) {
    poColor col = hsv2rgba(hsv);
    set(col.R, col.G, col.B, col.A);
}

poColor &poColor::set(float r, float g, float b, float a) {
	R = r; 
	G = g;
	B = b;
	A = a;
	return *this;
}

std::string poColor::toString() const {
	return (boost::format("%f %f %f %f")%R%G%B%A).str();
}


poHSVColor::poHSVColor() 
:   H(0.f)
,   S(0.f)
,   V(0.f) 
{}

poHSVColor::poHSVColor(float h, float s, float v)
:   H(h)
,   S(s)
,   V(v) 
{}

poHSVColor::poHSVColor(const poColor &col) {
    poHSVColor hcol = rgba2hsv(col);
    set(hcol.H, hcol.S, hcol.V);
}

poHSVColor &poHSVColor::set(float h, float s, float v) {
    H = h;
    S = s;
    V = v;
    return *this;
}

std::ostream &operator<<(std::ostream &o, const poColor &c) {
	o << c.toString();
	return o;
}

std::istream &operator>>(std::istream &i, poColor &c) {
	i >> c.R >> c.G >> c.B >> c.A;
	return i;
}
