/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poTween.h"
#include "poObject.h"
#include "Helpers.h"

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
		double now = getTime();
		double dt = now - last_time;
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

poTweenBase& poTweenBase::setDuration(double dur) {
	duration = dur;
	return *this;
}

poTweenBase& poTweenBase::setDelay(double del) {
	delay = del;
	return *this;
}

poTweenBase& poTweenBase::setExtraValues(double e1, double e2) {
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
	
	last_time = getTime();
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
