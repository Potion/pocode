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

#include "poRect.h"
#include "poMath.h"

#include <sstream>

using namespace std;

//General functions
namespace po {
    Point alignInRect(Point max, Rect rect, Alignment align) {
        Point offset;
        switch(align) {
            case PO_ALIGN_TOP_LEFT:
                offset.set(0.f, max.y - 1.f, 0.f);
                break;
            case PO_ALIGN_TOP_CENTER:
                offset.set((max.x - 1.f)/2.f, max.y - 1.f, 0.f);
                break;
            case PO_ALIGN_TOP_RIGHT:
                offset.set(max.x - 1.f, max.y - 1.f, 0.f);
                break;
            case PO_ALIGN_CENTER_LEFT:
                offset.set(0.f, (max.y - 1.f)/2.f, 0.f);
                break;
            case PO_ALIGN_CENTER_CENTER:
                offset.set((max.x - 1.f)/2.f, (max.y - 1.f)/2.f, 0.f);
                break;
            case PO_ALIGN_CENTER_RIGHT:
                offset.set(max.x - 1.f, (max.y - 1.f)/2.f, 0.f);
                break;
            case PO_ALIGN_BOTTOM_LEFT:
                offset.set(0.f, 0.f, 0.f);
                break;
            case PO_ALIGN_BOTTOM_CENTER:
                offset.set((max.x - 1.f)/2.f, 0.f, 0.f);
                break;
            case PO_ALIGN_BOTTOM_RIGHT:
                offset.set(max.x - 1.f, 0.f, 0.f);
                break;
            case PO_ALIGN_NONE:
                break;
        }
        return offset;
    }


    //------------------------------------------------------------------
    //Rect
    #pragma mark Rect
    Rect::Rect() 
    :	x(0)
    ,	y(0)
    ,   width(0)
    ,   height(0)
    {}

    Rect::Rect(float x, float y, float w, float h)
    :	x(x)
    ,	y(y)
    ,   width(w)
    ,   height(h)
    {}

    Rect::Rect(const Point &p1, const Point &p2)
    :	x(p1.x)
    ,   y(p1.y)
    ,	width(p2.x)
    ,   height(p2.y)
    {}
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Setters ============================================
    #pragma mark - Setters -

    //------------------------------------------------------------------------
    Rect& Rect::setPosition(float x, float y) {
        this->x = x;
        this->y = y;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect& Rect::setPosition(Point pos) {
        x = pos.x;
        y = pos.y;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::getPosition() const {
        return Point(x,y);
    }
    
    
    //------------------------------------------------------------------------
    Rect& Rect::setSize(float width, float height) {
        this->width = width;
        this->height = height;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect& Rect::setSize(Point size) {
        width = size.x;
        height = size.y;
        return *this;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Getters ============================================
    #pragma mark - Getters -

    //------------------------------------------------------------------------
    Point Rect::getSize() const {
        return Point(width, height);
    }
    
    
    //------------------------------------------------------------------------
    float Rect::getArea() const {
        return width * height;
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::getCenter() const {
        return Point(width/2.f + x, height/2.f+y);
    }
    
    
    //------------------------------------------------------------------------
    float Rect::getAspect() const {
        return width / height;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::set(float x, float y, float w, float h) {
        setPosition(x,y);
        setSize(w,h);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::set(Point pos, Point size) {
        setPosition(pos);
        setSize(size);
        return *this;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Expansion ========================================
    #pragma mark - Expansion -

    //------------------------------------------------------------------------
    Rect &Rect::include(float x, float y) {
        Point maxPoint = getPosition() + getSize();
        
        Point pos = getPosition();
        
        maxPoint.x  = std::max(x, maxPoint.x);
        maxPoint.y  = std::max(y, maxPoint.y);
        pos.x       = std::min(x, pos.x);
        pos.y       = std::min(y, pos.y);
        width       = maxPoint.x-pos.x;
        height      = maxPoint.y-pos.y;
        
        setPosition(pos);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::include(const Point &pt) {
        return include(pt.x, pt.y);
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::include(const Rect &rect) {
        include(rect.getPosition());
        return include(rect.getPosition() + rect.getSize());
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Scaling ==========================================
    #pragma mark - Scaling -

    //------------------------------------------------------------------------
    Rect &Rect::scale(float scalar) {
        Point size = getSize();
        size *= scalar;
        setSize(size);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::scale(float scalar, const Point &pt) {
        setPosition((getPosition() - pt) * scalar + pt);
        setSize(getSize() * scalar);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::inset(Point p) {
        setPosition(getPosition() + p);
        setSize(getSize() - (p *2));
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Rect &Rect::expand(Point p) {
        setPosition(getPosition() - p);
        setSize(getSize() + (p*2));
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::remap(Rect from, Point p) {
        Point from_ll = from.getTopLeft();
        Point from_ur = from.getBottomRight();
        
        Point to_ll = getTopLeft();
        Point to_ur = getBottomRight();
        
        Point pt;
        pt.x = Mapf(from_ll.x, from_ur.x, p.x, to_ll.x, to_ur.x);
        pt.y = Mapf(from_ll.y, from_ur.y, p.y, to_ll.y, to_ur.y);
        return pt;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Querying =========================================
    #pragma mark - Querying -

    //------------------------------------------------------------------------
    bool Rect::contains(float x, float y) const {
        return	x >= this->x && 
                x <= (this->x+width) && 
                y >= this->y && 
                y <= (this->y+height);
    }
    
    
    //------------------------------------------------------------------------
    bool Rect::contains(const Point &pt) const {
        return contains(pt.x, pt.y);
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Corners ==========================================
    #pragma mark - Corners -

    //------------------------------------------------------------------------
    std::vector<Point> Rect::getCorners() const {
        std::vector<Point> response;
        response.push_back(getTopLeft());
        response.push_back(getTopRight());
        response.push_back(getBottomRight());
        response.push_back(getBottomLeft());
        return response;
    }
    
    //------------------------------------------------------------------------
    Point Rect::getTopLeft() const {
        return getPosition();
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::getBottomLeft() const {
        return Point(x, y + height);
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::getTopRight() const {
        return Point(x+width, y);
    }
    
    
    //------------------------------------------------------------------------
    Point Rect::getBottomRight() const {
        return getPosition() + getSize();
    }
    
    
    //------------------------------------------------------------------------
    std::string Rect::toString() const {
        std::stringstream ss;
        ss << "rect(" << x << "," << y << "," << width << "," << height << ")";
        return ss.str();
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ ToString =========================================
    #pragma mark - ToString -

    //------------------------------------------------------------------------
    bool Rect::fromString(const std::string &str) {
        float x, y, w, h;
        int wrote = sscanf(str.c_str(), "rect(%f,%f,%f,%f)", &x, &y, &w, &h);
        if(wrote < 4) {
            set(0,0,0,0);
            return false;
        }
        
        set(x,y,w,h);
        return true;
    }
    
    
    //------------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &o, const Rect &r) {
        o << r.toString();
        return o;
    }
    
}/* End po namespace */