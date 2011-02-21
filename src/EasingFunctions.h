/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

inline bool goToFunc(float *v, float bv, float ev, 
			  double t, double b, double e, double d) 
{
	*v = (*v * d) + (ev * (1-d));
	return fabs(*v - ev) > (ev-bv)*.01f;
}

inline bool linearFunc(float *v, float bv, float ev, 
				double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2 + bv;
	return (t-b) <= d;
}

inline bool quadInFunc(float *v, float bv, float ev, 
				double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2 + bv;
	return (t-b) <= d;
}

inline bool quadOutFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * t2*(t2-2) + bv;
	return (t-b) <= d;
}

inline bool quadInOutFunc(float *v, float bv, float ev, 
				   double t, double b, double e, double d) 
{
	float t2 = (t - b) / d / 2.f;
	if(t2 < 1.f) 
		*v = (ev-bv)/2.f * t2*t2 + bv;
	else {
		t2--;
		*v = -(ev-bv)/2.f * (t2*(t2-2.f)-1.f) + b;
	}
	return (t-b) <= d;
}

inline bool cubeInFunc(float *v, float bv, float ev, 
				double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) <= d;
}

inline bool cubeOutFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2+1.f) + bv;
	return (t-b) <= d;
}

inline bool cubeInOutFunc(float *v, float bv, float ev, 
				   double t, double b, double e, double d) 
{
	float t2 = (t - b) / d / 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * (t2*t2*t2) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2 + 2.f) + bv;
	}
	return (t-b) <= d;
}

inline bool quartInFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) <= d;
}

inline bool quartOutFunc(float *v, float bv, float ev, 
				  double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = -(ev-bv) * (t2*t2*t2*t2-1.f) + bv;
	return (t-b) <= d;
}

inline bool quartInOutFunc(float *v, float bv, float ev, 
					double t, double b, double e, double d) 
{
	float t2 = (t - b) / d / 2.f;
	if(t < 1.f)
		*v = (ev-bv)/2.f * t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		return -(ev-bv)/2.f * (t2*t2*t2*t2-2.f) + bv;
	}
	return (t-b) <= d;
}

inline bool quintInFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2*t2*t2 + bv;
	return (t-b) <= d;
}


inline bool quintOutFunc(float *v, float bv, float ev, 
				  double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2*t2*t2+1.f) + bv;
	return (t-b) <= d;
}

inline bool quintInOutFunc(float *v, float bv, float ev, 
					double t, double b, double e, double d) 
{
	float t2 = (t - b) / d / 2.f;
	if(t2 < 1)
		*v = (ev-bv)/2.f * t2*t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2*t2*t2+2.f) + bv;
	}
	return (t-b) <= d;
}

inline bool sinusInFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * ::cosf(t2 * M_HALFPI) + (ev-bv) + bv;
	return (t-b) <= d;
}

inline bool sinusOutFunc(float *v, float bv, float ev, 
				  double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::sin(t2 * M_HALFPI) + bv;
	return (t-b) <= d;
}

inline bool sinusInOutFunc(float *v, float bv, float ev, 
					double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv)/2.f * (::cosf(M_PI*t2) - 1.f) + bv;
	return (t-b) <= d;
}

inline bool expoInFunc(float *v, float bv, float ev, 
				double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::powf(2, 10*(t2-1.f)) + bv;
	return (t-b) <= d;
}

inline bool expoOutFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * (-::powf(2, -10*t2) + 1.f) + bv;
	return (t-b) <= d;
}

inline bool expoInOutFunc(float *v, float bv, float ev, 
				   double t, double b, double e, double d) 
{
	float t2 = (t - b) / d / 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * ::powf(2.f, 10 * (t2-1.f)) + bv;
	else {
		t2--;
		*v = (ev-bv)/2.f * (-::powf(2, -10*t2) + 2.f) + bv;
	}
	return (t-b) <= d;
}

inline bool circInFunc(float *v, float bv, float ev, 
				double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * (::sqrtf(1.f - t2*t2) - 1.f) + bv;
	return (t-b) <= d;
}

inline bool circOutFunc(float *v, float bv, float ev, 
				 double t, double b, double e, double d) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * ::sqrtf(1.f - t2*t2) + bv;
	return (t-b) <= d;
}

inline bool circInOutFunc(float *v, float bv, float ev, 
				   double t, double b, double e, double d) 
{
	float t2 = (t - d) / d / 2.f;
	if(t2 < 1)
		*v = -(ev-bv)/2.f * (::sqrt(1.f - t2*t2) - 1.f) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (::sqrtf(1.f - t2*t2) + 1.f) + bv;
	}
	return (t-b) <= d;
}