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

#include "poEnums.h"
#include "poPoint.h"

#include <vector>

// CLASS NOTES
//
// Rect is a light-weight object used to specify bounding boxes and other rectangular forms
// throughout pocode.
//
// Rect is not an Object and hence cannot be added to the scene graph. See RectShape.
// 
// 



namespace po {
    class Rect;
    
    poPoint alignInRect(poPoint max, Rect rect, poAlignment align);

    class Rect {
    public:
        
        // CONSTRUCTORS
        Rect();
        Rect(float x, float y, float w, float h);
        Rect(const poPoint &origin, const poPoint &size);

        // RECT SET METHODS
        Rect&         set(float x, float y, float w, float h);
        Rect&         set(poPoint pos, poPoint size);
        
        Rect&         setPosition(float x, float y);
        Rect&         setPosition(poPoint pos);
        Rect&         setSize(float w, float h);
        Rect&         setSize(poPoint size);
        
        // RECT PROPERTIES
        poPoint         getPosition() const;
        poPoint         getSize() const;
        float           getArea() const;
        poPoint         getCenter() const;
        float           getAspect() const;
        
        // RECT CORNERS
        poPoint         getTopLeft() const;
        poPoint         getBottomLeft() const;
        poPoint         getTopRight() const;
        poPoint         getBottomRight() const;
        std::vector<poPoint> getCorners() const;

        // RECT EXPANSION
        Rect&         include(float x, float y);
        Rect&         include(const poPoint &pt);
        Rect&         include(const Rect &rect);
        
        // RECT SCALING
        Rect&         scale(float scalar);
        // scale around a point
        Rect&         scale(float scalar, const poPoint &pt);
        Rect&         inset(poPoint p);
        Rect&			expand(poPoint p);
        
        // RECT QUERYING
        bool            contains(float x, float y) const;
        bool            contains(const poPoint &pt) const;

        poPoint         remap(Rect from, poPoint p);
        
        // RECT TO STRING
        std::string     toString() const;
        bool            fromString(std::string const& str);
        
        float           x,y,width,height;
    };

    std::ostream &operator<<(std::ostream &o, const Rect &r);
    
}
