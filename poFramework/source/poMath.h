/*
 *  poMath.h
 *  poFramework3
 *
 *  Created by Joshua Fisher on 3/27/08.
 *  Copyright 2008 Potion Design, LLC. All rights reserved.
 *
 */

#include <cmath>
#include <cstdio>
#include <cstring>
#include "poPoint.h"

#ifndef __poMath_h__
#define __poMath_h__

void	set_identity(float* mat, int n);
bool	gaussian_solve(float* A, float* b, int n);
float	guassian_determinant(float* A, int n);
bool	gaussian_invert(float* A, int n);

void	perspective_matrix(float fov, float aspect, float near, float far, float m[16]);
void	transpose_matrix(float* m, int n);

bool	almostEqual(float a, float b, float tolerance=0.0001f);

float	dcos( float degree );
float	dsin( float degree );
float	dtan( float degree );
float	datan2( float y, float x );

template<typename T>
T	lerp(T A, T B, float p)
{
	return A*(1.0f-p) + B*p;
}


template<typename T>
T	clamp(T val, T lower, T upper)
{
	if(val < lower) return lower;
	if(val > upper) return upper;
	return val;
}

#ifndef WIN32
template<typename T>
T	max(T a, T b) {return a > b ? a : b;}
template<typename T>
T	min(T a, T b) {return a < b ? a : b;}
#endif

float poRand();
float poRand(float max);
float poRand(float min, float max);

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C );

#endif