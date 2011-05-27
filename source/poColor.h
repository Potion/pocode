/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

class poHSVColor;

class poColor
{
public:
	float R, G, B, A;
	
	poColor();
	poColor(float r, float g, float b, float a=1.f);
    poColor(const poHSVColor &hsv);
	
	poColor &set(float r, float g, float b, float a=1.f);
	std::string toString() const;
    
    static const poColor white;
	static const poColor black;
	static const poColor grey;
	static const poColor red;
	static const poColor yellow;
	static const poColor orange;
	static const poColor blue;
	static const poColor green;
};


class poHSVColor {
public:
	float H, S, V;
	
	poHSVColor();
	poHSVColor(float h, float s, float v);
    poHSVColor(const poColor &col);
    
    poHSVColor &set(float h, float s, float v);
};

static std::ostream &operator<<(std::ostream &o, const poColor &c) {
	o << c.toString();
	return o;
}

static std::istream &operator>>(std::istream &i, poColor &c) {
	i >> c.R >> c.G >> c.B >> c.A;
	return i;
}
