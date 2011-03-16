/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include <cmath>
#include <ctime>
#include <algorithm>

#include "poPoint.h"

#define M_2PI (M_PI*2.0)
#define M_HALFPI (M_PI/2.0)

inline bool almostEqual(float f1, float f2, float range=0.01f) {
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

inline float poRand(int low, int high) {
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

