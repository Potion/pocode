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


// CLASS NOTES
//
// Color represents a color in RGBA format. It is one of the most commonly used
// classes in pocode.
//
// Color assumes RGBA values are between 0.0 and 1.0. To set a Color using
// 0 to 255 values, use the set255() method. Values will be converted to the
// standard range.
//
// A Color may be set using HSV by creating a HSVColor and passing it into
// the Color constructor, or by calling the setHSV() method.
//

namespace po {
    class HSVColor;
    
    class Color {
    public:
        float R, G, B, A;
        
        // CONSTRUCTORS
        // Construct a default Color (0,0,0,0).
        Color();
        Color(Color const& c);
        // Constructs a new Color with values between 0.0 and 1.0.
        Color(float r, float g, float b, float a=1.f);
        // Constructs a new Color based upon an HSVColor.
        Color(const HSVColor &hsv);
        // Constructs a new color with alpha mulitplied by multAlpha.
        Color(Color c, float multAlpha);
        Color& operator=(Color const& c);
        
        // SET COLOR
        // Set with values between 0.0 and 1.0.
        Color&            set(float r, float g, float b, float a=1.f);
        // Set with another Color;
        Color&            set(const Color &fromColor);
        // Set with values between 0 and 255. Values will be scaled to 0.0 to 1.0.
        Color&            set255(float r, float g, float b, float a=255.f);
        // Set using HSV (HUE, SATURATION, VALUE) and alpha.
        Color&            setHSV(float h, float s, float v, float a=1.f);
        // Set with a string. Format can be "#ffffff" or "rgb(255,255,255)" or "rgb(255,255,255,255)".
        bool                set(const std::string &str);

        // STRING FROM COLOR
        std::string         toString() const;
        
        // PRE-DEFINED COLORS
        // See Color.cpp for actual color values.
        static const Color white;
        static const Color black;
        static const Color ltGrey;
        static const Color grey;
        static const Color dkGrey;
        static const Color red;
        static const Color yellow;
        static const Color orange;
        static const Color blue;
        static const Color green;
        static const Color cyan;
        static const Color magenta;
        static const Color transparent;

        static Color random();
    };

    bool operator==(const Color &c1, const Color &c2);
    bool operator!=(const Color &c1, const Color &c2);

    // CLASS NOTES
    //
    // HSVColor represents a color in HSV format. 
    //
    // Note that HSVColor is not a sub-class of Color.
    // A Color may be constructed from a HSVColor.
    // 


    class HSVColor {
    public:
        float H, S, V;
        
        HSVColor();
        HSVColor(float h, float s, float v);
        HSVColor(const Color &col);
        
        HSVColor&         set(float h, float s, float v);
    };


    std::ostream &operator<<(std::ostream &o, const Color &c);

    // hashPointerForColor is a convenience method for generator a color from an arbitrary pointer
    // This is often useful in debugging to different objects that have no inherent color.
    Color hashPointerForColor(void *ptr);

    // blendColors is used to blend two colors together based upon a percent between 0.0 and 1.0.
    // Colors are blended in the HSV color space for better color quality.
    Color blendColors(Color c1, Color c2, float pct);

} /*End po namespace */


