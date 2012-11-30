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

#include "poMath.h"
#include "poPoint.h"

#include <sstream>

namespace po {

    Point::Point()
    :	x(0.f), y(0.f), z(0.f)
    {}

    Point::Point(const float *f)
    :	x(f[0]), y(f[1]), z(f[2])
    {}

    Point::Point(float x, float y, float z)
    :	x(x), y(y), z(z)
    {}


    //------------------------------------------------------------------------
    Point &Point::set(Point p) {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        return *this;
    }


    //------------------------------------------------------------------------
    Point &Point::set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        return *this;
    }


    //------------------------------------------------------------------------
    Point &Point::setPolar(float deg, float mag) {
        x = cos_deg(deg) * mag;
        y = sin_deg(deg) * mag;
        return *this;
    }



    // -----------------------------------------------------------------------------------
    // ================================ Utils ============================================
    #pragma mark - Utils -

    //------------------------------------------------------------------------
    bool Point::fromString(std::string const& str) {
        int count = sscanf(str.c_str(), "Point(%f,%f,%f)", &x, &y, &z);
        if(count == 2)
            z = 0.f;
        else if(count < 2) {
            x = 0;
            y = 0;
            z = 0;
            return false;
        }
        return true;
    }


    //------------------------------------------------------------------------
    std::string Point::toString() const {
        std::stringstream ss;
        ss << "Point(" << x << "," << y << "," << z << ")";
        return ss.str();
    }


    //------------------------------------------------------------------------
    Point normalize(Point pt) {
        float len = pt.getLength();
        return Point(pt.x/len, pt.y/len, pt.z/len);
    }


    //------------------------------------------------------------------------
    Point &Point::normalize() {
        float len = getLength();
        if(len == 0) return *this;
        
        if(x != 0) x /= len;
        if(y != 0) y /= len;
        if(z != 0) z /= len;
        return *this;
    }


    //------------------------------------------------------------------------
    bool compare(Point a, Point b, float range) {
        return (compare(a.x, b.x, range) &&
                compare(a.y, b.y, range) &&
                compare(a.z, b.z, range));
    }



    // -----------------------------------------------------------------------------------
    // ================================ Getters ==========================================
    #pragma mark - Getters -

    //------------------------------------------------------------------------
    float Point::getLength() const {
        return sqrtf(x*x + y*y + z*z);
    }


    //------------------------------------------------------------------------
    float Point::getLengthSquared() const {
        return x*x + y*y + z*z;
    }


    //------------------------------------------------------------------------
    float Point::getDist(Point pt) {
        return Dist(x, y, z, pt.x, pt.y, pt.z);
    }


    //------------------------------------------------------------------------
    float Point::getDot(Point rhs) const {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }


    //------------------------------------------------------------------------
    Point Point::getCross(Point rhs) const {
        return Point(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
    }


    //------------------------------------------------------------------------
    Point Point::getNormal2D() const {
        return Point(y, -x);
    }


    //------------------------------------------------------------------------
    Point Point::getRotate2D(float deg) const {
        Point Xv(cos_deg(deg), sin_deg(deg));
        Point Yv(-sin_deg(deg), cos_deg(deg));
        return Point(Xv * x + Yv * y);
    }

    // -----------------------------------------------------------------------------------
    // ================================ Operators ========================================
    #pragma mark - Operators -

    //------------------------------------------------------------------------
    Point operator+(Point A, Point B) {
        return Point(A.x+B.x, A.y+B.y, A.z+B.z);
    }


    //------------------------------------------------------------------------
    Point operator-(Point A, Point B) {
        return Point(A.x-B.x, A.y-B.y, A.z-B.z);
    }


    //------------------------------------------------------------------------
    Point operator-(Point A) {
        return Point(-A.x, -A.y, -A.z);
    }


    //------------------------------------------------------------------------
    Point operator*(Point A, float Scalar) {
        return Point(A.x*Scalar, A.y*Scalar, A.z*Scalar);
    }


    //------------------------------------------------------------------------
    Point operator*(float S, Point A) {
        return operator*(A,S);
    }


    //------------------------------------------------------------------------
    Point operator*(Point A, Point B) {
        return Point(A.x*B.x, A.y*B.y, A.z*B.z);
    }


    //------------------------------------------------------------------------
    Point operator/(Point A, float Scalar) {
        return Point(A.x/Scalar, A.y/Scalar, A.z/Scalar);
    }


    //------------------------------------------------------------------------
    bool Point::operator==(Point rhs) {
        return compare(*this, rhs);
    }


    //------------------------------------------------------------------------
    void Point::operator+=(Point rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }


    //------------------------------------------------------------------------
    void Point::operator-=(Point rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }


    //------------------------------------------------------------------------
    void Point::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }


    //------------------------------------------------------------------------
    void Point::operator/=(float scalar) {
        if(scalar == 0) return;
        
        if(x != 0) x /= scalar;
        if(y != 0) y /= scalar;
        if(z != 0) z /= scalar;
    }


    //------------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &o, const Point &p) {
        o << p.toString();
        return o;
    }
} /*End po namespace */