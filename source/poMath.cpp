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

//
//  poMath.cpp
//  pocode
//
//  Created by Joshua Fisher on 4/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMath.h"

namespace po {
    
    //------------------------------------------------------------------------
    inline float determinant(Point row1, Point row2, Point row3) {
        return (row1.x*row2.y*row3.z + row1.y*row2.z*row3.x + row1.z*row2.x*row3.y -
                row1.x*row2.z*row3.y - row1.y*row2.x*row3.z - row1.z*row2.y*row3.x);
    }
    
    
    //------------------------------------------------------------------------
    bool rayIntersection(Ray r1, Ray r2, Point *p1, Point *p2) {
        Point c1 = r1.dir.getCross(r2.dir);
        float len_sqr = c1.getLengthSquared();
        
        if(compare(len_sqr, 0.f)) {
            return false;
        }
        
        Point diff = r2.origin - r1.origin;
        float t1 = determinant(diff, r2.dir, c1) / len_sqr;
        float t2 = determinant(diff, r1.dir, c1) / len_sqr;
        
        *p1 = r1.origin + r1.dir*t1;
        *p2 = r2.origin + r2.dir*t2;
        
        return *p1 == *p2;
    }
    
    
    //------------------------------------------------------------------------
    float angleBetweenPoints(Point a, Point b, Point c) {
        Point ab = a - b;
        Point cb = c - b;
        
        // perp dot product
        float ret = atan2f(ab.x*cb.y - ab.y*cb.x, ab.x*cb.x + ab.y*cb.y);
        
        return ret;
    }
    
    
    //------------------------------------------------------------------------
    bool pointInTriangle( Point &P, Point &A, Point &B, Point &C ) {
        Point AB = A - B;
        Point BC = B - C;
        Point PA = P - A;
        
        if ( BC.x*AB.y - BC.y*AB.x > 0) {   // test winding
            if ( PA.x*AB.y - PA.y*AB.x > 0 )
                return false;
            Point PB = P - B;
            if ( PB.x*BC.y - PB.y*BC.x > 0 )
                return false;
            Point CA = C - A;
            Point PC = P - C;
            if ( PC.x*CA.y - PC.y*CA.x > 0 )
                return false;
        }
        else {
            if ( PA.x*AB.y - PA.y*AB.x < 0 )
                return false;
            Point PB = P - B;
            if ( PB.x*BC.y - PB.y*BC.x < 0 )
                return false;
            Point CA = C - A;
            Point PC = P - C;
            if ( PC.x*CA.y - PC.y*CA.x < 0 )
                return false;
        }
        
        return true;
    }
    
    
    //------------------------------------------------------------------------
    bool pointInTriangle3D( Point P, MatrixSet& M, Point A, Point B, Point C ) {
        A = M.localToGlobal(A);
        B = M.localToGlobal(B);
        C = M.localToGlobal(C);
        
        return pointInTriangle( P, A, B, C );
    }
    
    
    //------------------------------------------------------------------------
    bool pointInRect3D( Point P, MatrixSet& M, Rect R ) {
        Point A( R.x, R.y, 0 );
        Point B( R.x+R.width, R.y, 0 );
        Point C( R.x+R.width, R.y+R.height, 0 );
        Point D( R.x, R.y+R.height, 0 );
        
        if ( pointInTriangle3D( P, M, A, B, C ) )
            return true;
        if ( pointInTriangle3D( P, M, C, D, A ) )
            return true;
        return false;
    }
    
    
    //------------------------------------------------------------------------
    Point projectOntoLine(Point p, Point a, Point b) {
        const float l2 = (a-b).getLengthSquared();
        const float t = (p-a).getDot(b-a) / l2;
        return a + t * (b - a);
    }
    
    
    //------------------------------------------------------------------------
    // http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    float distanceToLine(Point p, Point a, Point b) {
        const float l2 = (a-b).getLengthSquared();
        if(l2 == 0.f)
            // a == b
            return p.getDist(a);
        
        const float t = (p-a).getDot(b-a) / l2;
        if(t < 0.f)
            // beyond a
            return p.getDist(a);
        else if(t > 1.f)
            // beyond b
            return p.getDist(b);
        
        const Point projection = a + t * (b - a);
        return p.getDist(projection);
    }
} /* End po Namespace */