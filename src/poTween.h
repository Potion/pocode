/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poInclude.h"

class poObject;

enum poTweenMinorType {
	PO_TWEEN_EASE_IN = 0,
	PO_TWEEN_EASE_OUT,
	PO_TWEEN_EASE_IN_OUT,
	
	PO_TWEEN_MINOR_NUM_OPTIONS
};

enum poTweenMajorType {
	PO_TWEEN_LINEAR = 0,
	PO_TWEEN_QUADRADIC,
	PO_TWEEN_CUBIC,
	PO_TWEEN_QUARTIC,
	PO_TWEEN_QUINTIC,
	PO_TWEEN_SINUSOIDAL,
	PO_TWEEN_EXPONENTIAL,
	PO_TWEEN_CIRCULAR,
	
	PO_TWEEN_MAJOR_NUM_OPTIONS
};

enum poTweenRepeat {
	PO_TWEEN_REPEAT_NONE		= 0,
	PO_TWEEN_REPEAT_REGULAR		= 1,
	PO_TWEEN_REPEAT_PINGPONG	= 2,
	
	PO_TWEEN_REPEAT_DELAY		= 1 << 8
};

class poTween {
public:
	typedef boost::function<void ()> TweenFinishedCallback_t;
	// return false to stop
	typedef bool (*TweenFunction)(float *v, float bv, float ev, 
								  double t, double b, double e, double d);
	
	poTween();
	poTween(float *value, poTweenRepeat repeat=PO_TWEEN_REPEAT_NONE, int repeat_count=-1);
	
	poTween& setValuePtr(float *value);
	poTween& setRepeat(poTweenRepeat repeat, int count=-1);
	
	// send a message when a tween finishes
	poTween& notifyOnComplete(poObject* obj, const std::string &str);
	poTween& notifyOnComplete(TweenFinishedCallback_t callback);

	// move the animation forward
	void update();
	
	poTween& start();
	// optionally jump to the end state
	poTween& stop(bool jump=false);
	
	bool isRunning() const;
	bool isComplete() const;
	
	poTween& tween(poTweenMajorType major, poTweenMinorType minor, 
				   float end_val, float duration, float delay=0.f);
	
private:
	void startWithDelay(bool do_delay);
	
	typedef std::vector<TweenFinishedCallback_t> MessageVec;
	MessageVec completion_messages;
		
	enum {
		NOT_RUNNING,
		RUNNING,
		COMPLETE
	} state;
	
	float *value, begin_value, end_value;
	TweenFunction tween_func;
	float last_time, time, duration, begin, end, delay;
	int repeat_count, repeat_counter;
	poTweenRepeat repeat;
};



