/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poMath.h"

/*	args:
	1. v	= ptr to the value to be modified
	2. bv	= start value
	3. ev	= end value
	4. t	= time right now
	5. beginning time
	6. ending time
	7. duration
	8. extra value for specific easing functions
	9. extra value for specific easing functions
 
	return:
	true if tween is finished
 */

inline bool goToFunc(float *v, float bv, float ev, 
					 float t, float b, float e, float d,
					 float e1, float e2)
{
	*v = (*v * e1) + (ev * (1-e1));
	return ::fabs(*v - ev) < e2;
}

inline bool linearFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2 + bv;
	return (t-b) >= d;
}

inline bool quadInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2 + bv;
	return (t-b) >= d;
}

inline bool quadOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * t2*(t2-2) + bv;
	return (t-b) >= d;
}

inline bool quadInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f) 
		*v = (ev-bv)/2.f * t2*t2 + bv;
	else {
		t2--;
		*v = -(ev-bv)/2.f * (t2*(t2-2.f)-1.f) + bv;
	}
	return (t-b) >= d;
}

inline bool cubeInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) >= d;
}

inline bool cubeOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2+1.f) + bv;
	return (t-b) >= d;
}

inline bool cubeInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * (t2*t2*t2) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2 + 2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool quartInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) >= d;
}

inline bool quartOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = -(ev-bv) * (t2*t2*t2*t2-1.f) + bv;
	return (t-b) >= d;
}

inline bool quartInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t < 1.f)
		*v = (ev-bv)/2.f * t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		return -(ev-bv)/2.f * (t2*t2*t2*t2-2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool quintInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2*t2*t2 + bv;
	return (t-b) >= d;
}


inline bool quintOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2*t2*t2+1.f) + bv;
	return (t-b) >= d;
}

inline bool quintInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1)
		*v = (ev-bv)/2.f * t2*t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2*t2*t2+2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool sinusInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * ::cosf(t2 * M_HALF_PI) + (ev-bv) + bv;
	return (t-b) >= d;
}

inline bool sinusOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::sin(t2 * M_HALF_PI) + bv;
	return (t-b) >= d;
}

inline bool sinusInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv)/2.f * (::cosf(M_PI*t2) - 1.f) + bv;
	return (t-b) >= d;
}

inline bool expoInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::powf(2, 10*(t2-1.f)) + bv;
	return (t-b) >= d;
}

inline bool expoOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * (-::powf(2, -10*t2) + 1.f) + bv;
	return (t-b) >= d;
}

inline bool expoInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * ::powf(2.f, 10 * (t2-1.f)) + bv;
	else {
		t2--;
		*v = (ev-bv)/2.f * (-::powf(2, -10*t2) + 2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool circInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * (::sqrtf(1.f - t2*t2) - 1.f) + bv;
	return (t-b) >= d;
}

inline bool circOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * ::sqrtf(1.f - t2*t2) + bv;
	return (t-b) >= d;
}

inline bool circInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - d) / d * 2.f;
	if(t2 < 1)
		*v = -(ev-bv)/2.f * (::sqrtf(1.f - t2*t2) - 1.f) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (::sqrtf(1.f - t2*t2) + 1.f) + bv;
	}
	return (t-b) >= d;
}