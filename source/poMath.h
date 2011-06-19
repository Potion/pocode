/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include <cmath>
#include <ctime>
#include <algorithm>

#include "poPoint.h"

#define M_2PI (M_PI*2.0)
#define M_HALF_PI (M_PI/2.0)
#define M_QUARTER_PI (M_HALF_PI/2.0)

struct poExtrudedLineSeg { 
	poPoint p1, p2, p3, p4;
	poExtrudedLineSeg();
	poExtrudedLineSeg(poPoint a, poPoint b, float w);
	poExtrudedLineSeg(poPoint ul, poPoint ll, poPoint ur, poPoint lr);
};

struct poRay {
	poPoint origin, dir;
	poRay(poPoint o, poPoint d);
};

// cheap 'matrix' determinant
float determinant(poPoint row1, poPoint row2, poPoint row3);
// angle ABC
float angleBetweenPoints(poPoint a, poPoint b, poPoint c);
// angle ABC but with extruded line segments
float angleBetweenSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2);
// returns false if parallel, p1 = intersection or p1 and p2 = closest point on each
bool rayIntersection(poRay r1, poRay r2, poPoint *p1, poPoint *p2);
// returns true if top is on the outside
bool combineExtrudedLineSegments(poExtrudedLineSeg seg1, poExtrudedLineSeg seg2, poPoint *top, poPoint *bottom);

inline bool compare(float f1, float f2, float range=1.0e-6f) {
	return ::fabs(f1-f2) < range;
}

inline float deg2rad(float deg) {
	return deg / 180.0 * M_PI;
}

inline float rad2deg(float rad) {
	return rad / M_PI * 180.0;
}

inline float sin_deg(float deg) {
	return ::sinf(deg2rad(deg));
}

inline float cos_deg(float deg) {
	return ::cosf(deg2rad(deg));
}

inline void poRandSeed() {
	::srand((unsigned int)::time(NULL));
}

inline void poRandSeed(int seed) {
	::srand(seed);
}

inline float poRand() {
	return rand() / (float)INT_MAX;
}

inline float poRand(float low, float high) {
	return poRand() * (high-low) + low;
}

inline void capToRange(float *f, float low, float high) {
	*f = std::max(*f, low);
	*f = std::min(*f, high);
}

inline void capToRange(poPoint *pt, float low, float high) {
	capToRange(&pt->x, low, high);
	capToRange(&pt->y, low, high);
	capToRange(&pt->z, low, high);
}

// will round -.5 to -1
inline float roundSym(float f) {
	return (f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5);
}

inline float round(float f) {
	return floor(f + 0.5);
}

inline poPoint round(poPoint p) {
	return poPoint(round(p.x), round(p.y), round(p.z));
}

inline std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*p1 + 2*invt*t*control + t*t*p2;
		response.push_back(pt);
	}
	return response;
}

inline std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*invt*p1 + 3*invt*invt*t*c1 + 3*invt*t*t*c2 + t*t*t*p2;
		response.push_back(pt);
	}
	return response;
}

inline float curveLength(const std::vector<poPoint> &curve) {
	float len = 0;
	for(int i=0; i<curve.size()-1; i++) {
		len += (curve[i+1] - curve[i]).length();
	}
	return len;
}

template <typename T>
inline T clamp(T min, T max, T val) {
	return std::max(std::min(val, max), min);
}

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C );


