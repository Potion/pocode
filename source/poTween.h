/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poPoint.h"
#include "poEasingFunctions.h"

enum poTweenRepeat {
	PO_TWEEN_REPEAT_NONE,
	PO_TWEEN_REPEAT_REGULAR,
	PO_TWEEN_REPEAT_PINGPONG
};

class poObject;

class poTweenBase {
public:
	typedef boost::function<void()> poTweenFinishedCallback;
	
	typedef bool (*poTweenFunction)(float *val, float begin_val, float end_val,
									float time, float begin_time, float end_time, float duration,
									float extra1, float extra2);
	
	poTweenBase();
	virtual ~poTweenBase();
	
	void update();
	
	poTweenBase& setRepeat(poTweenRepeat type, int count=-1);
	poTweenBase& setNotification(poObject *obj, const std::string &msg);
	poTweenBase& clearNotifications();
	poTweenBase& setTweenFunction(poTweenFunction func);
	poTweenBase& setDuration(float duration);
	poTweenBase& setDelay(float delay);
	poTweenBase& setExtraValues(float e1, float e2);
	
	poTweenBase& start();
	poTweenBase& stop(bool jump_to_end=false);
	
	bool isRunning() const;
	bool isComplete() const;
	bool isRepeating() const;
	int getRepeatCount() const;
	
protected:
	void reset();
	
	// these exist to mitigate the different types
	virtual void setValueToBegin() = 0;
	virtual void setValueToEnd() = 0;
	virtual void swapBeginAndEnd() = 0;
	virtual void slewBeginValue() = 0;
	// returns true when the tween is complete
	virtual bool updateTweenWithTime(float time, float begin, float end, 
									 float duration, float e1, float e2) = 0;
	
	poTweenFunction tween_func;
	
private:
	void startWithDelay(bool do_delay);
	
	enum {
		NOT_RUNNING,
		RUNNING,
		COMPLETE
	} state;
	
	int repeat_count, repeat_counter;
	poTweenRepeat repeat_type;
	
	float time, last_time;
	float begin_time, end_time, duration, delay;
	float extra1, extra2;
	
	std::vector<poTweenFinishedCallback> callbacks;
};


class poFloatTween
:	public poTweenBase
{
public:
	poFloatTween(float* addr);
	// will stop the tween if its running
	poTweenBase& set(float ev);
	
protected:
	virtual void setValueToBegin();
	virtual void setValueToEnd();
	virtual void swapBeginAndEnd();
	virtual void slewBeginValue();
	virtual bool updateTweenWithTime(float time, float begin, float end, 
									 float duration, float e1, float e2);
	
private:
	float *value;
	float begin_value, end_value;
};

class poPointTween
:	public poTweenBase
{
public:
	poPointTween(poPoint* addr);
	// will stop the tween if its running
	poTweenBase& set(poPoint ev);
	
protected:
	virtual void setValueToBegin();
	virtual void setValueToEnd();
	virtual void swapBeginAndEnd();
	virtual void slewBeginValue();
	virtual bool updateTweenWithTime(float time, float begin, float end, 
									 float duration, float e1, float e2);
	
private:
	poPoint *value;
	poPoint begin_value, end_value;
};
