/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poTween.h"
#include "poObject.h"

#include <mach/mach_time.h>
float nowTime() {
	static mach_timebase_info_data_t info;
	if(info.denom == 0)
		mach_timebase_info(&info);
	return float(mach_absolute_time() * info.numer / info.denom * 1e-9);
}

poTweenBase::poTweenBase()
:	state(NOT_RUNNING)
,	repeat_type(PO_TWEEN_REPEAT_NONE)
,	repeat_count(0)
,	repeat_counter(0)
,	tween_func(goToFunc)
,	duration(0.f)
,	delay(0.f)
,	extra1(0.99f)
,	extra2(0.001f)
,	time(0.0)
{}

poTweenBase::~poTweenBase() {}

void poTweenBase::update() {
	if(!isRunning())
		return;
	
	if(tween_func) {
		float now = nowTime();
		float dt = now - last_time;
		last_time = now;
		
		time += dt;
		if(time < begin_time) {
			slewBeginValue();
			return;
		}
		
		// will return true when tween is finished
		if(updateTweenWithTime(time, begin_time, end_time, duration, extra1, extra2)) {
			setValueToEnd();
			BOOST_FOREACH(const poTweenFinishedCallback &callback, callbacks) {
				callback();
			}
			
			bool repeat_ok = repeat_count < 0 || repeat_counter < repeat_count;
			repeat_counter++;
			
			if(repeat_type & PO_TWEEN_REPEAT_REGULAR && repeat_ok) {
				setValueToBegin();
				startWithDelay(false);
			}
			else if(repeat_type & PO_TWEEN_REPEAT_PINGPONG && repeat_ok) {
				setValueToEnd();
				swapBeginAndEnd();
				startWithDelay(false);
			}
			else {
				state = COMPLETE;
			}
		}
	}
}

poTweenBase& poTweenBase::setRepeat(poTweenRepeat type, int count) {
	repeat_type = type;
	repeat_count = count;
	return *this;
}

poTweenBase& poTweenBase::setNotification(poObject *obj, const std::string &msg, const poDictionary &/*dict*/) {
	poDictionary dict;
	dict.setPtr("tween", this);
	callbacks.push_back(boost::bind(&poObject::messageHandler, obj, msg, dict));
	return *this;
}

poTweenBase& poTweenBase::clearNotifications() {
	callbacks.clear();
	return *this;
}

poTweenBase& poTweenBase::setTweenFunction(poTweenFunction func) {
	tween_func = func;
	return *this;
}

poTweenBase& poTweenBase::setDuration(float dur) {
	duration = dur;
	return *this;
}

poTweenBase& poTweenBase::setDelay(float del) {
	delay = del;
	return *this;
}

poTweenBase& poTweenBase::setExtraValues(float e1, float e2) {
	extra1 = e1;
	extra2 = e2;
	return *this;
}

poTweenBase& poTweenBase::start() {
	if(state != RUNNING) {
		repeat_counter = 0;
		startWithDelay(true);
	}
	return *this;
}

poTweenBase& poTweenBase::stop(bool jump_to_end) {
	if(state == RUNNING) {
		state = NOT_RUNNING;
		if(jump_to_end)
			setValueToEnd();
	}
	return *this;
}

bool poTweenBase::isRunning() const {
	return state == RUNNING;
}

bool poTweenBase::isComplete() const {
	return state == COMPLETE;
}

bool poTweenBase::isRepeating() const {
	return repeat_type != PO_TWEEN_REPEAT_NONE;
}

int poTweenBase::getRepeatCount() const {
	return repeat_counter;
}

void poTweenBase::reset() {
	state = NOT_RUNNING;
}

void poTweenBase::startWithDelay(bool do_delay) {
	state = RUNNING;
	
	last_time = nowTime();
	time = 0;
	
	if(do_delay) {
		begin_time = delay;
		end_time = begin_time + duration;
	}
	else {
		begin_time = 0;
		end_time = duration;
	}
}




poFloatTween::poFloatTween(float* addr)
:	value(addr)
{}

poTweenBase& poFloatTween::set(float ev) {
	reset();
	begin_value = *value;
	end_value = ev;
	return *this;
}

void poFloatTween::setValueToBegin()	{*value = begin_value;}
void poFloatTween::setValueToEnd()	{*value = end_value;}
void poFloatTween::swapBeginAndEnd()	{std::swap(begin_value, end_value);}
void poFloatTween::slewBeginValue()	{begin_value = *value;}

bool poFloatTween::updateTweenWithTime(float time, float begin, float end, 
									   float duration, float e1, float e2) 
{
	return tween_func(value, begin_value, end_value,
					  time, begin, end, duration, e1, e2);
}






poPointTween::poPointTween(poPoint* addr)
:	value(addr)
{}

poTweenBase& poPointTween::set(poPoint ev) {
	reset();
	begin_value = *value;
	end_value = ev;
	return *this;
}

void poPointTween::setValueToBegin()	{*value = begin_value;}
void poPointTween::setValueToEnd()		{*value = end_value;}
void poPointTween::swapBeginAndEnd()	{std::swap(begin_value, end_value);}
void poPointTween::slewBeginValue()		{begin_value = *value;}

bool poPointTween::updateTweenWithTime(float time, float begin, float end, 
									   float duration, float e1, float e2) 
{
	bool done1 = tween_func(&value->x, begin_value.x, end_value.x,
							time, begin, end, duration, e1, e2);
	bool done2 = tween_func(&value->y, begin_value.y, end_value.y,
							time, begin, end, duration, e1, e2);
	bool done3 = tween_func(&value->z, begin_value.z, end_value.z,
							time, begin, end, duration, e1, e2);

	return done1 && done2 && done3;
}


