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
    
    Point alignInRect(Point max, Rect rect, Alignment align);

    class Rect {
    public:
        
        // CONSTRUCTORS
        Rect();
        Rect(float x, float y, float w, float h);
        Rect(const Point &origin, const Point &size);

        // RECT SET METHODS
        Rect&       set(float x, float y, float w, float h);
        Rect&       set(Point pos, Point size);
        
        Rect&       setPosition(float x, float y);
        Rect&       setPosition(Point pos);
        Rect&       setSize(float w, float h);
        Rect&       setSize(Point size);
        
        // RECT PROPERTIES
        Point       getPosition() const;
        Point       getSize() const;
        float       getArea() const;
        Point       getCenter() const;
        float       getAspect() const;
        
        // RECT CORNERS
        Point       getTopLeft() const;
        Point       getBottomLeft() const;
        Point       getTopRight() const;
        Point       getBottomRight() const;
        std::vector<Point> getCorners() const;

        // RECT EXPANSION
        Rect&       include(float x, float y);
        Rect&       include(const Point &pt);
        Rect&       include(const Rect &rect);
        
        // RECT SCALING
        Rect&       scale(float scalar);
        // scale around a point
        Rect&       scale(float scalar, const Point &pt);
        Rect&       inset(Point p);
        Rect&       expand(Point p);
        
        // RECT QUERYING
        bool        contains(float x, float y) const;
        bool        contains(const Point &pt) const;
        bool        contains(const po::Rect rect) const;
        
        bool        overlaps(const po::Rect rect) const;

        Point       remap(Rect from, Point p);
        
        // RECT TO STRING
        std::string     toString() const;
        bool            fromString(std::string const& str);
        
        float           x,y,width,height;
    };

    std::ostream &operator<<(std::ostream &o, const Rect &r);
    
}
