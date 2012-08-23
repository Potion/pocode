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

#include "poColor.h"
#include "poMath.h"
#include "poHelpers.h"

#include <boost/format.hpp>

#define min3(a,b,c) a < b ? (a < c ? a : c) : (b < c ? b : c)
#define max3(a,b,c) a > b ? (a > c ? a : c) : (b > c ? b : c)


const poColor poColor::white = poColor(1,1,1);
const poColor poColor::black = poColor(0,0,0);
const poColor poColor::ltGrey = poColor(.75, .75, .75);
const poColor poColor::grey = poColor(.5, .5, .5);
const poColor poColor::dkGrey = poColor(.25, .25, .25);
const poColor poColor::red = poColor(1,0,0);
const poColor poColor::yellow = poColor(1,1,0);
const poColor poColor::orange = poColor(1.0, 0.5, 0.2);
const poColor poColor::blue = poColor(0,0,1);
const poColor poColor::green = poColor(0,1,0);
const poColor poColor::cyan = poColor(0,1,1);
const poColor poColor::magenta = poColor(1,0,1);
const poColor poColor::transparent = poColor(0,0,0,0);

poColor poColor::random() {
	return poColor(poRand(), poRand(), poRand(), 1.f);
}

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
	
	return poHSVColor(H/360.f,S,V);
}

poColor hsv2rgba(poHSVColor hsv) {
	float h6 = (hsv.H*360) / 60.0;
	
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
,   A(0.f)
{}

poColor::poColor(poColor c, float multAlpha)
:	R(c.R)
,	G(c.G)
,	B(c.B)
,	A(c.A*multAlpha)
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

poColor::poColor(poColor const& c)
:	R(c.R)
,	G(c.G)
,	B(c.B)
,	A(c.A)
{}

poColor& poColor::operator=(poColor const& c) {
	if(this != &c) {
		R = c.R;
		G = c.G;
		B = c.B;
		A = c.A;
	}
	return *this;
}

poColor& poColor::set(float r, float g, float b, float a) {
	R = r; 
	G = g;
	B = b;
	A = a;
	return *this;
}

poColor& poColor::set255(float r, float g, float b, float a) {
	R = r / 255.f;
	G = g / 255.f;
	B = b / 255.f;
	A = a / 255.f;
	return *this;
}

poColor& poColor::setHSV(float h, float s, float v, float a) {
    *this = hsv2rgba( poHSVColor(h,s,v) );
    A = a;
	return *this;
}

poColor &poColor::set(poColor &fromColor) {
	R = fromColor.R;
	G = fromColor.G;
	B = fromColor.B;
	A = fromColor.A;
	return *this;
}

bool poColor::set(const std::string &str) {
	int r,g,b,a;
	
	int written = sscanf(str.c_str(), "#%2x%2x%2x%2x", &r,&g,&b,&a);
	
	if(written == 4)		{set255(r,g,b,a); return true;}
	else if(written == 3)	{set255(r,g,b,255); return true;}
	
	written = sscanf(str.c_str(), "rgb(%d,%d,%d)", &r,&g,&b);
	if(written == 3)		{set255(r,g,b,255); return true;}
	
	written = sscanf(str.c_str(), "rgba(%d,%d,%d,%d)", &r,&g,&b,&a);
	if(written == 4)		{set255(r,g,b,a); return true;}
	
	R = G = B = A = 0.f;
	return false;
}

std::string poColor::toString() const {
	return (boost::format("rgba(%f,%f,%f,%f)")%int(R*255)%int(G*255)%int(B*255)%int(A*255)).str();
}

bool operator==(const poColor &c1, const poColor &c2) {
	return compare(c1.R, c2.R) && compare(c1.G, c2.G) && compare(c1.B, c2.B) && compare(c1.A, c2.A);
}

bool operator!=(const poColor &c1, const poColor &c2) {
	return !(c1 == c2);
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

poColor hashPointerForColor(void *ptr) {
	unsigned long val = (long)ptr;
	
	val = (val+0x7ed55d16) + (val<<12);
	val = (val^0xc761c23c) ^ (val>>19);
	val = (val+0x165667b1) + (val<<5);
	val = (val+0xd3a2646c) ^ (val<<9);
	val = (val+0xfd7046c5) + (val<<3);
	val = (val^0xb55a4f09) ^ (val>>16);

	GLubyte r = (val>>0) & 0xFF;
	GLubyte g = (val>>8) & 0xFF;
	GLubyte b = (val>>16) & 0xFF;
	
	GLubyte max = r>g ? (r>b ? r : b) : (g>b ? g : b);
	
	int mult = 127;
	int add = 63;
	r = (r*mult)/max + add;
	g = (g*mult)/max + add;
	b = (b*mult)/max + add;
	
	return poColor().set255(r,g,b);
}

poColor blendColors(poColor c1, poColor c2, float pct) {
	poHSVColor h1(c1), h2(c2);
	float h = h1.H * pct + h2.H * (1.f-pct);
	float s = h1.S * pct + h2.S * (1.f-pct);
	float v = h1.V * pct + h2.V * (1.f-pct);
	return poColor(poHSVColor(h,s,v));
}
