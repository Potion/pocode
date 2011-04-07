/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poColor.h"
#include "poMath.h"

#define min3(a,b,c) a < b ? (a < c ? a : c) : (b < c ? b : c)
#define max3(a,b,c) a > b ? (a > c ? a : c) : (b > c ? b : c)

poHSVColor rgba2hsv(poColor rgba) {
	float r = rgba.red * rgba.alpha;
	float g = rgba.green * rgba.alpha;
	float b = rgba.blue * rgba.alpha;
	
	float min = min3(r,g,b);
	float max = max3(r,g,b);
	float range = max - min;
	float val = max;
	float sat = 0.f;
	float hue = 0.f;
	
	if(max != 0)
		sat = range / max;
	
	if(sat != 0) {
		if(rgba.red == max)
			hue = (rgba.green - rgba.blue) / range;
		else if(rgba.green == max)
			hue = 2 + (rgba.blue - rgba.red) / range;
		else
			hue = 4 + (rgba.red - rgba.green) / range;
		
		hue *= 60.f;
		if(hue < 0.f)
			hue += 360.f;
	}
	
	return poHSVColor(hue,sat,val);
}

poColor hsv2rgba(poHSVColor hsv) {
	float h6 = hsv.hue / 60.0;
	
	int i = (int)floorf(h6);
	float f = h6 - i;
	float p = hsv.value * (1.0 - hsv.saturation);
	float q = hsv.value * (1.0 - (hsv.saturation * f));
	float t = hsv.value * (1.0 - (hsv.saturation * (1.0 - f)));
	float v = hsv.value;
	
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
:	red(0.f)
,   green(0.f)
,   blue(0.f)
,   alpha(1.f)
{}

poColor::poColor(float r, float g, float b, float a)
:	red(r)
,   green(g)
,   blue(b)
,   alpha(a)
{}

poColor::poColor(const poHSVColor &hsv) {
    poColor col = hsv2rgba(hsv);
    set(col.red, col.green, col.blue, col.alpha);
}

poColor &poColor::set(float r, float g, float b, float a) {
	red = r; 
	green = g;
	blue = b;
	alpha = a;
	return *this;
}

poHSVColor::poHSVColor() 
:   hue(0.f)
,   saturation(0.f)
,   value(0.f) 
{}

poHSVColor::poHSVColor(float h, float s, float v)
:   hue(h)
,   saturation(s)
,   value(v) 
{}

poHSVColor::poHSVColor(const poColor &col) {
    poHSVColor hcol = rgba2hsv(col);
    set(hcol.hue, hcol.saturation, hcol.value);
}

poHSVColor &poHSVColor::set(float h, float s, float v) {
    hue = h;
    saturation = s;
    value = v;
    return *this;
}
