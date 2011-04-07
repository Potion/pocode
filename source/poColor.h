/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

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
	
	HSVColor() : hue(0.f), saturation(0.f), value(0.f) {}
	HSVColor(float h, float s, float v) : hue(h), saturation(s), value(v) {}
    HSVColor &set(float h, float s, float v);
};
