/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poTween.h"
#include "poObject.h"
#include "poHelpers.h"

#undef min
#undef max

poTweenBase::poTweenBase()
:	state(NOT_RUNNING)
,	repeatType(PO_TWEEN_REPEAT_NONE)
,	repeatCount(0)
,	repeatCounter(0)
,	tweenFunc(goToFunc)
,	duration(0.f)
,	delay(0.f)
,	extra1(0.99f)
,	extra2(0.001f)
,	time(0.0)
,	delay_on_repeat(false)
{}

poTweenBase::~poTweenBase() {}

void poTweenBase::update() {
	if(!isRunning())
		return;
	
	if(tweenFunc) {
		float now = poGetElapsedTime();
		float dt = now - lastTime;
		lastTime = now;
		
		time += dt;
		if(time < beginTime) {
			slewBeginValue();
			return;
		}
		
		// will return true when tween is finished
		if(updateTweenWithTime(time, beginTime, endTime, duration, extra1, extra2)) {
			setValueToEnd();
			
			bool repeat_ok = repeatCount < 0 || repeatCounter < repeatCount;
			repeatCounter++;
			
			if(repeatType & PO_TWEEN_REPEAT_REGULAR && repeat_ok) {
				setValueToBegin();
				startWithDelay(delay_on_repeat);
			}
			else if(repeatType & PO_TWEEN_REPEAT_PINGPONG && repeat_ok) {
				setValueToEnd();
				swapBeginAndEnd();
				startWithDelay(delay_on_repeat);
			}
			else {
				state = COMPLETE;
			}
			
			if(callback) {
				poTweenFinishedCallback cb = callback;
				poTweenFinishedCallback().swap(callback);
				cb();
			}
		}
	}
}

poTweenBase& poTweenBase::setRepeat(poTweenRepeat type, int count) {
	repeatType = type;
	repeatCount = count;
	return *this;
}

poTweenBase& poTweenBase::setNotification(poObject *obj, const std::string &msg, const poDictionary &d) {
	if(obj) {
		poDictionary dict = d;
		dict.set("tween", this);
		callback = boost::bind(&poObject::messageHandler, obj, msg, dict);
	}
	else {
		poTweenFinishedCallback().swap(callback);
	}
	return *this;
}

poTweenBase& poTweenBase::setTweenFunction(poTweenFunction func) {
	tweenFunc = func;
	return *this;
}

poTweenBase& poTweenBase::setDuration(float dur) {
	duration = dur;
	return *this;
}

poTweenBase& poTweenBase::setDelay(float del, bool d) {
	delay = del;
	delay_on_repeat = d;
	return *this;
}

poTweenBase& poTweenBase::setExtraValues(float e1, float e2) {
	extra1 = e1;
	extra2 = e2;
	return *this;
}

poTweenBase& poTweenBase::start() {
	if(state != RUNNING) {
		repeatCounter = 0;
		startWithDelay(true);
	}
	return *this;
}

poTweenBase& poTweenBase::stop(bool jumpToEnd) {
	if(state == RUNNING) {
		state = NOT_RUNNING;
		
		if(jumpToEnd)
			setValueToEnd();

		poTweenFinishedCallback().swap(callback);
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
	return repeatType != PO_TWEEN_REPEAT_NONE;
}

int poTweenBase::getRepeatCount() const {
	return repeatCounter;
}

float poTweenBase::getProgress() const {
	return (float)std::min(1.f, std::max(0.f, (time - beginTime - delay) / duration));
}

void poTweenBase::reset() {
	state = NOT_RUNNING;
}

void poTweenBase::startWithDelay(bool doDelay) {
	state = RUNNING;
	
	lastTime = poGetElapsedTime();
	time = 0;
	
	if(doDelay) {
		beginTime = delay;
		endTime = beginTime + duration;
	}
	else {
		beginTime = 0;
		endTime = duration;
	}
}
