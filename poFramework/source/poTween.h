/*
 *  poTween.h
 *  poFramework_static_lib
 *
 *  Created by Yasmin Elayat on 8/14/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PO_TWEEN_STYLE_
#define _PO_TWEEN_STYLE_


#include "poObjectModifier.h"
#include "poObject.h"

#ifdef _WIN32
	float   getStartTime();
#else
	void	startMasterTimer();
	void	incrementMasterTimer();
#endif

float	getCurrentTime();

enum poTweenStyle { PO_LINEAR_TWEEN =0, PO_EASE_IN_TWEEN, PO_EASE_OUT_TWEEN, PO_EASE_IN_OUT_TWEEN, PO_JITTER_TWEEN, PO_OVERSHOOT_TWEEN, PO_LAZY_EASE_IN_OUT_TWEEN };

class poTween : public poObjectModifier
{
	public:

	poTween();
	poTween( poPoint startPos, poPoint endPos, poTweenStyle style, float duration, float emphasis=1.0 );

	void	set( poPoint startPos, poPoint endPos, poTweenStyle style, float duration, float emphasis=1.0 );
	void	set( poPoint startScale, poPoint endScale);

	virtual void setup(poObject*);
	
#ifdef _WIN32
	void	start() { getStartTime(); isPlaying = true; timeParam = adjustment = 0; };
#else
	void	start() { startTime = getCurrentTime(); isPlaying = true; timeParam = adjustment = 0; };
#endif	

	void	pause() { isPlaying = false; };
	void	reset() { start(); };

	poTweenStyle	style;
	float			emphasis;
	float			duration;

	float			startTime;
	bool			isPlaying;

	float			timeParam;
	float			adjustment;

	poPoint startPosition, endPosition;
	poPoint startScale, endScale;
	float	startRotation, endRotation;
};


#endif
