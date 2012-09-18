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

#pragma once

#include <string>
#include <ostream>

class poHSVColor;

// CLASS NOTES
//
// poColor represents a color in RGBA format. It is one of the most commonly used
// classes in pocode.
//
// poColor assumes RGBA values are between 0.0 and 1.0. To set a poColor using
// 0 to 255 values, use the set255() method. Values will be converted to the
// standard range.
//
// A poColor may be set using HSV by creating a poHSVColor and passing it into
// the poColor constructor, or by calling the setHSV() method.
//

class poColor {
public:
	float R, G, B, A;
	
    // CONSTRUCTORS
    // Construct a default poColor (0,0,0,0).
	poColor();
	poColor(poColor const& c);
    // Constructs a new poColor with values between 0.0 and 1.0.
	poColor(float r, float g, float b, float a=1.f);
    // Constructs a new poColor based upon an poHSVColor.
    poColor(const poHSVColor &hsv);
	// Constructs a new color with alpha mulitplied by multAlpha.
	poColor(poColor c, float multAlpha);
	poColor& operator=(poColor const& c);
	
    // SET COLOR
    // Set with values between 0.0 and 1.0.
	poColor&            set(float r, float g, float b, float a=1.f);
    // Set with another poColor;
    poColor&            set(poColor &fromColor);
    // Set with values between 0 and 255. Values will be scaled to 0.0 to 1.0.
	poColor&            set255(float r, float g, float b, float a=255.f);
    // Set using HSV (HUE, SATURATION, VALUE) and alpha.
    poColor&            setHSV(float h, float s, float v, float a=1.f);
	// Set with a string. Format can be "#ffffff" or "rgb(255,255,255)" or "rgb(255,255,255,255)".
	bool                set(const std::string &str);

    // STRING FROM COLOR
	std::string         toString() const;
    
    // PRE-DEFINED COLORS
    // See poColor.cpp for actual color values.
    static const poColor white;
	static const poColor black;
    static const poColor ltGrey;
	static const poColor grey;
    static const poColor dkGrey;
	static const poColor red;
	static const poColor yellow;
	static const poColor orange;
	static const poColor blue;
	static const poColor green;
    static const poColor cyan;
    static const poColor magenta;
	static const poColor transparent;

	static poColor random();
};

bool operator==(const poColor &c1, const poColor &c2);
bool operator!=(const poColor &c1, const poColor &c2);

// CLASS NOTES
//
// poHSVColor represents a color in HSV format. 
//
// Note that poHSVColor is not a sub-class of poColor.
// A poColor may be constructed from a poHSVColor.
// 


class poHSVColor {
public:
	float H, S, V;
	
	poHSVColor();
	poHSVColor(float h, float s, float v);
    poHSVColor(const poColor &col);
    
    poHSVColor&         set(float h, float s, float v);
};


std::ostream &operator<<(std::ostream &o, const poColor &c);

// hashPointerForColor is a convenience method for generator a color from an arbitrary pointer
// This is often useful in debugging to different objects that have no inherent color.
poColor hashPointerForColor(void *ptr);

// blendColors is used to blend two colors together based upon a percent between 0.0 and 1.0.
// Colors are blended in the HSV color space for better color quality.
poColor blendColors(poColor c1, poColor c2, float pct);




