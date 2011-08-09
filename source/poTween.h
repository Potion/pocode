/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poEnums.h"
#include "poPoint.h"
#include "poDictionary.h"
#include "poEasingFunctions.h"

class poObject;

class poTweenBase {
public:
	typedef boost::function<void()> poTweenFinishedCallback;
	
	typedef bool (*poTweenFunction)(float *val, float begin_val, float end_val,
									double time, double begin_time, double end_time, double duration,
									double extra1, double extra2);
	
	poTweenBase();
	virtual ~poTweenBase();
	
	void update();
	
	poTweenBase& setRepeat(poTweenRepeat type, int count=-1);
	// cancelling notification:
	//		passing obj=NULL
	//		calling 'stop'
	// notifications only happen once
	// reregister in the notification callback if you want to get one for the next repeat
	poTweenBase& setNotification(poObject *obj, const std::string &msg="", const poDictionary &dict=poDictionary());
	poTweenBase& setTweenFunction(poTweenFunction func);
	poTweenBase& setDuration(double duration);
	// default will not take delay into account when repeating
	poTweenBase& setDelay(double delay, bool delayOnRepeat=false);
	poTweenBase& setExtraValues(double e1, double e2);
	
	poTweenBase& start();
	poTweenBase& stop(bool jump_to_end=false);
	
	bool isRunning() const;
	bool isComplete() const;
	bool isRepeating() const;
	int getRepeatCount() const;
	// [0 .. 1] number won't be meaningful if tweenFunc = gotoFunc
	float getProgress() const;
	
protected:
	void reset();
	
	// these exist to mitigate the different types
	virtual void setValueToBegin() = 0;
	virtual void setValueToEnd() = 0;
	virtual void swapBeginAndEnd() = 0;
	virtual void slewBeginValue() = 0;
	// returns true when the tween is complete
	virtual bool updateTweenWithTime(double time, double begin, double end, 
									 double duration, double e1, double e2) = 0;
	
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
	
	double time, last_time;
	double begin_time, end_time, duration, delay;
	double extra1, extra2;
	
	bool delay_on_repeat;
	
	poTweenFinishedCallback callback;
};


static bool tweenUpdater(poTweenBase::poTweenFunction func, 
						 float *value, float begin_value, float end_value,
						 double time, double begin, double end,
						 double duration, double e1, double e2) 
{
	return func(value, begin_value, end_value, time, begin, end, duration, e1, e2);
}

static bool tweenUpdater(poTweenBase::poTweenFunction func, 
						 poPoint *value, poPoint begin_value, poPoint end_value,
						 double time, double begin, double end,
						 double duration, double e1, double e2) 
{
    bool A = func(&value->x, begin_value.x, end_value.x, time, begin, end, duration, e1, e2);
    bool B = func(&value->y, begin_value.y, end_value.y, time, begin, end, duration, e1, e2);
    bool C = func(&value->z, begin_value.z, end_value.z, time, begin, end, duration, e1, e2);
    return A && B && C;
}

static bool tweenUpdater(poTweenBase::poTweenFunction func, 
						 poColor *value, poColor begin_value, poColor end_value,
						 double time, double begin, double end,
						 double duration, double e1, double e2) 
{
	poHSVColor hsv1, hsv2(begin_value), hsv3(end_value);
	bool d1 = func(&hsv1.H, hsv2.H, hsv3.H, time, begin, end, duration, e1, e2);
	bool d2 = func(&hsv1.S, hsv2.S, hsv3.S, time, begin, end, duration, e1, e2);
	bool d3 = func(&hsv1.V, hsv2.V, hsv3.V, time, begin, end, duration, e1, e2);
	*value = poColor(hsv1);
	return d1 && d2 && d3;
}

template <typename T>
class poTween : public poTweenBase
{
public:
	typedef T value_type;

	poTween(value_type *addr) : value(addr) {}
	poTween &set(value_type end_value) {
		reset();
		begin = *value;
		end = end_value;
		return *this;
	}
	
protected:
	void setValueToBegin()	{*value = begin;}
	void setValueToEnd()	{*value = end;}
	void swapBeginAndEnd()	{std::swap(begin, end);}
	void slewBeginValue()	{begin = *value;}

	virtual bool updateTweenWithTime(double t, double b, double e, 
									 double d, double e1, double e2) 
	{
		return tweenUpdater(tween_func, value, begin, end, t, b, e, d, e1, e2);
	}

private:
	value_type *value;
	value_type begin, end;
};
