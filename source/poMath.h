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


#include <cmath>
#include <ctime>
#include <algorithm>

#include "poPoint.h"
#include "poRect.h"
#include "poColor.h"
#include "poMatrixSet.h"

#undef M_PI
#define M_PI 3.14159265f
#define M_2PI (M_PI*2.f)
#define M_HALF_PI (M_PI/2.f)
#define M_QUARTER_PI (M_HALF_PI/2.f)

namespace po {
    inline bool compare(float f1, float f2, float range=1.0e-6f) {
        return ::fabs(f1-f2) < range;
    }
    
    //------------------------------------------------------------------------
    inline float deg2rad(float deg) {
        return deg / 180.f * M_PI;
    }
    
    //------------------------------------------------------------------------
    inline float rad2deg(float rad) {
        return rad / M_PI * 180.f;
    }
    
    //------------------------------------------------------------------------
    inline float sin_deg(float deg) {
        return ::sinf(deg2rad(deg));
    }
    
    //------------------------------------------------------------------------
    inline float cos_deg(float deg) {
        return ::cosf(deg2rad(deg));
    }
    
    //------------------------------------------------------------------------
    inline void RandSeed() {
        ::srand((unsigned int)::time(NULL));
    }
    
    //------------------------------------------------------------------------
    inline void RandSeed(int seed) {
        ::srand(seed);
    }
    
    //------------------------------------------------------------------------
    inline float Rand() {
        return rand() / (float)RAND_MAX;
    }
    
    //------------------------------------------------------------------------
    inline float Rand(float low, float high) {
        return poRand() * (high-low) + low;
    }
    
    //------------------------------------------------------------------------
    inline Point randomPointInRect(poRect r) {
        return poPoint(poRand(r.x,r.x+r.width),
                       poRand(r.y,r.y+r.height));
    }
    
    //------------------------------------------------------------------------
    inline Color randomColor(float alpha=1.f) {
        return Color(Rand(), Rand(), Rand(), alpha);
    }
    
    //------------------------------------------------------------------------
    inline void capToRange(float *f, float low, float high) {
        *f = std::max(*f, low);
        *f = std::min(*f, high);
    }
    
    //------------------------------------------------------------------------
    inline void capToRange(Point *pt, float low, float high) {
        capToRange(&pt->x, low, high);
        capToRange(&pt->y, low, high);
        capToRange(&pt->z, low, high);
    }
    
    //------------------------------------------------------------------------
    // will round -.5 to -1
    inline float roundSym(float f) {
        return (f > 0.f) ? floor(f + 0.5f) : ceil(f - 0.5f);
    }
    
    //------------------------------------------------------------------------
    inline Point roundSym(Point p) {
        return Point(roundSym(p.x), roundSym(p.y), roundSym(p.z));
    }
    
    //------------------------------------------------------------------------
    inline float round(float f) {
        return floor(f + 0.5f);
    }
    
    //------------------------------------------------------------------------
    inline Point round(Point p) {
        return Point(round(p.x), round(p.y), round(p.z));
    }
    
    //------------------------------------------------------------------------
    inline Point floor(Point p) {
        return Point(floor(p.x), floor(p.y), floor(p.z));
    }
    
    //------------------------------------------------------------------------
    template <typename T>
    inline T Clamp(T min, T max, T val) {
        return std::max(std::min(val, max), min);
    }
    
    //------------------------------------------------------------------------
    inline float Mapf(float min1, float max1, float v, float min2, float max2) {
        float tmp = (v-min1) / (max1-min1);
        return tmp * (max2-min2) + min2;
    }
    
    //------------------------------------------------------------------------
    inline float Dist(float x1, float y1, float z1, float x2, float y2, float z2) {
        return sqrtf(powf((x2-x1), 2) + powf((y2-y1), 2) + powf((z2-z1), 2));
    }
    
    //------------------------------------------------------------------------
    inline float Dist(Point p1, Point p2) {
        return Dist(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
    }
    
    //------------------------------------------------------------------------
    struct Ray {
        Point origin, dir;
        Ray(Point o, Point d)
        : origin(o), dir(normalize(d)) {}
    };
    
    //------------------------------------------------------------------------
    // angle ABC
    float angleBetweenPoints(Point a, Point b, Point c);
    
    //------------------------------------------------------------------------
    // returns false if parallel, p1 = intersection or p1 and p2 = closest point on each
    bool rayIntersection(Ray r1, Ray r2, Point *p1, Point *p2);
    
    
    //------------------------------------------------------------------------
    //Implemented in CPP
    bool pointInTriangle( Point &P, Point &A, Point &B, Point &C );
    bool pointInTriangle3D( Point P, MatrixSet& M, Point A, Point B, Point C );
    bool pointInRect3D( Point P, MatrixSet& M, Rect R );

    Point projectOntoLine(Point p, Point a, Point b);
    float distanceToLine(Point p, Point a, Point b);
} /* End po Namespace */
