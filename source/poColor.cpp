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

namespace po {
    const Color Color::white = Color(1,1,1);
    const Color Color::black = Color(0,0,0);
    const Color Color::ltGrey = Color(.75, .75, .75);
    const Color Color::grey = Color(.5, .5, .5);
    const Color Color::dkGrey = Color(.25, .25, .25);
    const Color Color::red = Color(1,0,0);
    const Color Color::yellow = Color(1,1,0);
    const Color Color::orange = Color(1.0, 0.5, 0.2);
    const Color Color::blue = Color(0,0,1);
    const Color Color::green = Color(0,1,0);
    const Color Color::cyan = Color(0,1,1);
    const Color Color::magenta = Color(1,0,1);
    const Color Color::transparent = Color(0,0,0,0);

    Color Color::random() {
        return Color(poRand(), poRand(), poRand(), 1.f);
    }
    
    
    //------------------------------------------------------------------------
    HSVColor rgba2hsv(Color rgba) {
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
        
        return HSVColor(H/360.f,S,V);
    }
    
    
    //------------------------------------------------------------------------
    Color hsv2rgba(HSVColor hsv) {
        float h6 = (hsv.H*360) / 60.0;
        
        int i = (int)floorf(h6);
        float f = h6 - i;
        float p = hsv.V * (1.0 - hsv.S);
        float q = hsv.V * (1.0 - (hsv.S * f));
        float t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));
        float v = hsv.V;
        
        Color color;
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
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Color =====================================
    #pragma mark Class: Color
    
    Color::Color()
    :	R(0.f)
    ,   G(0.f)
    ,   B(0.f)
    ,   A(0.f)
    {}

    Color::Color(Color c, float multAlpha)
    :	R(c.R)
    ,	G(c.G)
    ,	B(c.B)
    ,	A(c.A*multAlpha)
    {}


    Color::Color(float r, float g, float b, float a)
    :	R(r)
    ,   G(g)
    ,   B(b)
    ,   A(a)
    {}

    Color::Color(const HSVColor &hsv) {
        Color col = hsv2rgba(hsv);
        set(col.R, col.G, col.B, col.A);
    }

    Color::Color(Color const& c)
    :	R(c.R)
    ,	G(c.G)
    ,	B(c.B)
    ,	A(c.A)
    {}

    
    Color& Color::operator=(Color const& c) {
        if(this != &c) {
            R = c.R;
            G = c.G;
            B = c.B;
            A = c.A;
        }
        return *this;
    }
    
    
    bool operator==(const Color &c1, const Color &c2) {
        return compare(c1.R, c2.R) && compare(c1.G, c2.G) && compare(c1.B, c2.B) && compare(c1.A, c2.A);
    }
    
    bool operator!=(const Color &c1, const Color &c2) {
        return !(c1 == c2);
    }
    
    //------------------------------------------------------------------------
    Color& Color::set(float r, float g, float b, float a) {
        R = r; 
        G = g;
        B = b;
        A = a;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Color& Color::set255(float r, float g, float b, float a) {
        R = r / 255.f;
        G = g / 255.f;
        B = b / 255.f;
        A = a / 255.f;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Color& Color::setHSV(float h, float s, float v, float a) {
        *this = hsv2rgba( HSVColor(h,s,v) );
        A = a;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Color &Color::set(Color &fromColor) {
        R = fromColor.R;
        G = fromColor.G;
        B = fromColor.B;
        A = fromColor.A;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    bool Color::set(const std::string &str) {
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
    
    
    //------------------------------------------------------------------------
    std::string Color::toString() const {
        return (boost::format("rgba(%f,%f,%f,%f)")%int(R*255)%int(G*255)%int(B*255)%int(A*255)).str();
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: HSVColor ==================================
    #pragma mark Class: HSVColor
    
    HSVColor::HSVColor()
    :   H(0.f)
    ,   S(0.f)
    ,   V(0.f) 
    {}

    HSVColor::HSVColor(float h, float s, float v)
    :   H(h)
    ,   S(s)
    ,   V(v) 
    {}

    HSVColor::HSVColor(const Color &col) {
        HSVColor hcol = rgba2hsv(col);
        set(hcol.H, hcol.S, hcol.V);
    }
    
    
    //------------------------------------------------------------------------
    HSVColor &HSVColor::set(float h, float s, float v) {
        H = h;
        S = s;
        V = v;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &o, const Color &c) {
        o << c.toString();
        return o;
    }
    
    
    //------------------------------------------------------------------------
    Color hashPointerForColor(void *ptr) {
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
        
        return Color().set255(r,g,b);
    }
    
    
    //------------------------------------------------------------------------
    Color blendColors(Color c1, Color c2, float pct) {
        HSVColor h1(c1), h2(c2);
        float h = h1.H * pct + h2.H * (1.f-pct);
        float s = h1.S * pct + h2.S * (1.f-pct);
        float v = h1.V * pct + h2.V * (1.f-pct);
        return Color(HSVColor(h,s,v));
    }
}/*End po namespace */