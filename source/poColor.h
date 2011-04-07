/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

class poHSVColor;

class poColor
{
public:
	float red, green, blue, alpha;
	
	poColor();
	poColor(float r, float g, float b, float a=1.f);
    poColor(const poHSVColor &hsv);
	
	poColor &set(float r, float g, float b, float a=1.f);
};


class poHSVColor {
public:
	float hue, saturation, value;
	
	poHSVColor();
	poHSVColor(float h, float s, float v);
    poHSVColor(const poColor &col);
    
    poHSVColor &set(float h, float s, float v);
};
