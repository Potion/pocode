/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

struct HSVColor {
	float hue, saturation, value;
	
	HSVColor() : hue(0.f), saturation(0.f), value(0.f) {}
	HSVColor(float h, float s, float v) : hue(h), saturation(s), value(v) {}
};

class poColor
{
public:
	float red, green, blue, alpha;
	
	poColor();
	poColor(float r, float g, float b, float a=1.f);
	
	poColor &set(float r, float g, float b, float a=1.f);
	poColor &setHSV(float h, float s, float v);
	poColor &setHSV(HSVColor hsv);
};

// will pre-multiply the alpha
HSVColor rgba2hsv(poColor rgba);
// will come back with alpha = 1.f
poColor hsv2rgba(HSVColor hsv);

