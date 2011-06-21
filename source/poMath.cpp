//
//  poMath.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMath.h"

inline float determinant(poPoint row1, poPoint row2, poPoint row3) {
	return (row1.x*row2.y*row3.z + row1.y*row2.z*row3.x + row1.z*row2.x*row3.y -
			row1.x*row2.z*row3.y - row1.y*row2.x*row3.z - row1.z*row2.y*row3.x);
}

bool rayIntersection(poRay r1, poRay r2, poPoint *p1, poPoint *p2) {
	poPoint c1 = r1.dir.cross(r2.dir);
	float len_sqr = c1.lengthSquared();
	
	if(compare(len_sqr, 0.f)) {
		return false;
	}
	
	poPoint diff = r2.origin - r1.origin;
	float t1 = determinant(diff, r2.dir, c1) / len_sqr;
	float t2 = determinant(diff, r1.dir, c1) / len_sqr;
	
	*p1 = r1.origin + r1.dir*t1;
	*p2 = r2.origin + r2.dir*t2;
	
	return *p1 == *p2;
}

float angleBetweenPoints(poPoint a, poPoint b, poPoint c) {
	poPoint ab = a - b;
	poPoint cb = c - b;
	
	// perp dot product
	float ret = atan2f(ab.x*cb.y - ab.y*cb.x, ab.x*cb.x + ab.y*cb.y);
	
	return ret;
}

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C )
{
    poPoint AB = A - B;
    poPoint BC = B - C;
    poPoint PA = P - A;
    
    if ( BC.x*AB.y - BC.y*AB.x > 0)    // test winding
    {
        if ( PA.x*AB.y - PA.y*AB.x > 0 )
            return false;
        poPoint PB = P - B;
        if ( PB.x*BC.y - PB.y*BC.x > 0 )
            return false;
        poPoint CA = C - A;
        poPoint PC = P - C;
        if ( PC.x*CA.y - PC.y*CA.x > 0 )
            return false;
    }
    else
    {
        if ( PA.x*AB.y - PA.y*AB.x < 0 )
            return false;
        poPoint PB = P - B;
        if ( PB.x*BC.y - PB.y*BC.x < 0 )
            return false;
        poPoint CA = C - A;
        poPoint PC = P - C;
        if ( PC.x*CA.y - PC.y*CA.x < 0 )
            return false;
    }
    
    return true;
}
