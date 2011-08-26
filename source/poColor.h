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
	// make a new color with alpha mulitplied by mult_alpha
	poColor(poColor c, float mult_alpha);
	
	poColor &set(float r, float g, float b, float a=1.f);
    poColor &set(poColor *fromColor);
	poColor &set255(float r, float g, float b, float a=255.f);
	// can either be #ffffff or rgb(255,255,255)
	bool	set(const std::string &str);

	std::string toString() const;
    
    static const poColor white;
	static const poColor black;
    static const poColor lt_grey;
	static const poColor grey;
    static const poColor dk_grey;
	static const poColor red;
	static const poColor yellow;
	static const poColor orange;
	static const poColor blue;
	static const poColor green;
    static const poColor cyan;
    static const poColor magenta;
};

class poHSVColor {
public:
	float H, S, V;
	
	poHSVColor();
	poHSVColor(float h, float s, float v);
    poHSVColor(const poColor &col);
    
    poHSVColor &set(float h, float s, float v);
};

std::ostream &operator<<(std::ostream &o, const poColor &c);

poColor hashPointerForColor(void *ptr);
poColor blendColors(poColor c1, poColor c2, float pct);


