
/*
 *  poTween.cpp
 *  poFramework_static_lib
 *
 *  Created by Yasmin Elayat on 8/14/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "poTween.h"

#ifdef _WIN32
	#include <time.h>
	#include <sys/timeb.h>
#endif


#ifdef _WIN32
	float totalMillis = 0;
	float currentMillis = 0;
	
	struct _timeb timeStart;
	struct _timeb timeEnd;
#else

	int		timerFrameCount = -1;
	int		lastTimeSec = 0;
	float	masterTimeDelta = 0.1;
	float	masterTime = 0.0;
	
#endif


#ifdef _WIN32

float getStartTime()
{
	_ftime64_s( &timeStart );

	int secondsTime = timeStart.time;
	totalMillis = (float)timeStart.millitm/1000.0 + (float)secondsTime;

	//printf("start time = %f\n",totalMillis);
	return totalMillis;
}


#else 

//must be called on every frame
void	incrementMasterTimer()
{
	int currentTimeSec = time(0);
	
	// initialize timer
	if ( timerFrameCount == -1 )
	{
		timerFrameCount = 0;
		lastTimeSec = currentTimeSec;
	}
	
	// every second update rate	
	if (  currentTimeSec != lastTimeSec )
	{
		if ( timerFrameCount != 0 )
			masterTimeDelta = (float) (currentTimeSec-lastTimeSec) / (float) timerFrameCount;
		else
			masterTimeDelta = 0.1;
		lastTimeSec = time(0);
		timerFrameCount = 0;
	}

	// increment time and count frames
	masterTime += masterTimeDelta;
	timerFrameCount++;
}

#endif

float	getCurrentTime()
{
	#ifdef _WIN32
		_ftime64_s( &timeEnd );
		int secondsTime = timeEnd.time - timeStart.time;
		totalMillis = (float)timeEnd.millitm/1000.0 + (float)secondsTime; 
		return totalMillis;
	#else
		return masterTime;
	#endif
}


poTween::poTween()
{
	this->startPosition.set( 0,0,0 );
	this->endPosition.set( 100,0,0 );
	this->startRotation = this->endRotation = 0;
	this->startScale.set( 1,1,1 );
	this->endScale.set( 1,1,1 );
	this->style = PO_LINEAR_TWEEN;
	this->duration = 1.0;
	this->emphasis = 1.0;
	
	startTime = 0;
	isPlaying = false;
}

poTween::poTween( poPoint startPos, poPoint endPos, poTweenStyle style, float duration, float emphasis )
{
	set( startPos, endPos, style, duration, emphasis );
}

void	poTween::set(  poPoint startPos, poPoint endPos, poTweenStyle S, float duration, float emphasis )
{
	this->startPosition = startPos;
	this->endPosition = endPos;
	this->style = S;
	this->duration = duration;
	this->emphasis = emphasis;
	
	startTime = 0; 
	isPlaying = false;
}

void poTween::set( poPoint startScale, poPoint endScale)
{
	this->startScale = startScale;
	this->endScale = endScale;
	this->startRotation = this->endRotation = 0;
}


void poTween::setup(poObject* obj)
{
	if ( isPlaying == false )
		return;
	
	// varies between 0.0 and 1.0 over 'duration' seconds
	#ifdef _WIN32
		float timeParam = (float)getCurrentTime() / duration; 
	#else
		float timeParam = (getCurrentTime() - startTime) / duration; 
	#endif
	
	if ( timeParam >= 1.0 )
	{
		timeParam = 1.0;
		isPlaying = false;
	}
	
	adjustment = timeParam;
	
	// p various betwee.n 0 and 1.0 as a parameterized function of t
	if ( style == PO_LINEAR_TWEEN )
		adjustment = timeParam;
	
	if ( style == PO_EASE_IN_TWEEN )
		adjustment = pow(timeParam, emphasis);
	
	if ( style == PO_EASE_OUT_TWEEN )
		adjustment = pow((float)timeParam, (float)1.0/emphasis);
	
	if ( style == PO_EASE_IN_OUT_TWEEN )
		adjustment = 0.5*cos(3.14159*(timeParam-1.0)) + 0.5;
	
	if ( style == PO_JITTER_TWEEN )
	{
		adjustment = timeParam + poRand()*emphasis;
		if ( timeParam >= 0.999 )
			adjustment = 1.0;
	}
	if( style == PO_LAZY_EASE_IN_OUT_TWEEN)
	{
		adjustment = 0.4*atan(3.14159*(1.959*timeParam-1.0)) + 0.5;
		if ( timeParam >= 1.0 )
			adjustment = 1.0;
	}
	
	if ( timeParam <= 0.0 )
		adjustment = 0.0;
	if ( timeParam >= 1.0 )
		adjustment = 1.0;
	
	obj->position = startPosition*(1.0-adjustment) + endPosition*adjustment;
	obj->rotation = startRotation*(1.0-adjustment) + endRotation*adjustment;
	obj->scale = startScale*(1.0-adjustment) + endScale*adjustment;
}
