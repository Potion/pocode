#include "poTween.h"
#include "poObject.h"

#include "EasingFunctions.h"

// the implementation has to be available at compile time for all units
#include <mach/mach_time.h>

double nowTime() {
	static mach_timebase_info_data_t info;
	if(info.denom == 0)
		mach_timebase_info(&info);
	return double(mach_absolute_time() * info.numer / info.denom * 1e-9);
}

static poTween::TweenFunction tweenFuncTable[PO_TWEEN_MAJOR_NUM_OPTIONS][PO_TWEEN_MINOR_NUM_OPTIONS] = {
	{linearFunc, linearFunc, linearFunc},
	{quadInFunc, quadOutFunc, quadInOutFunc},
	{cubeInFunc, cubeOutFunc, cubeInOutFunc},
	{quartInFunc, quartOutFunc, quartInOutFunc},
	{quintInFunc, quintOutFunc, quintInOutFunc},
	{sinusInFunc, sinusOutFunc, sinusInOutFunc},
	{expoInFunc, expoOutFunc, expoInOutFunc},
	{circInFunc, circOutFunc, circInOutFunc}
};

poTween::poTween()
:	state(NOT_RUNNING)
,	value(NULL)
,	tween_func(NULL)
,	repeat(PO_TWEEN_REPEAT_NONE)
,	repeat_count(0)
{}

poTween::poTween(float *value, poTweenRepeat repeat, int count)
:	state(NOT_RUNNING)
,	value(value)
,	tween_func(NULL)
,	repeat(repeat)
,	repeat_count(count)
{}

poTween& poTween::setValuePtr(float *value) {
	this->value = value;
	return *this;
}

poTween& poTween::setRepeat(poTweenRepeat repeat, int count) {
	this->repeat = repeat;
	this->repeat_count = count;
	return *this;
}

poTween& poTween::notifyOnComplete(poObject* obj, const std::string &str) {
	completion_messages.push_back(boost::bind(&poObject::messageHandler, obj, str, poDictionary()));
	return *this;
}

poTween& poTween::notifyOnComplete(TweenFinishedCallback_t callback) {
	completion_messages.push_back(callback);
	return *this;
}

void poTween::update() {
	if(state != RUNNING)
		return;
	
	double now = nowTime();
	double dt = now - last_time;
	last_time = now;
	
	time += dt;
	
	if(time < begin) {
		begin_value = *value;
		return;
	}
	
	if(!tween_func(value,begin_value,end_value,
				   time,begin,end,duration)) 
	{
		*value = end_value;
		BOOST_FOREACH(const TweenFinishedCallback_t &cb, completion_messages) {
			cb();
		}

		bool repeat_ok = !repeat_count || repeat_counter < repeat_count;
		repeat_counter++;

		if(repeat & PO_TWEEN_REPEAT_REGULAR && repeat_ok) {
			*value = begin_value;
			startWithDelay(repeat & PO_TWEEN_REPEAT_DELAY);
		}
		else if(repeat & PO_TWEEN_REPEAT_PINGPONG && repeat_ok) {
			*value = end_value;
			std::swap(begin_value, end_value);
			startWithDelay(repeat & PO_TWEEN_REPEAT_DELAY);
		}
		else {
			state = COMPLETE;
		}
	}
}

poTween& poTween::start() {
	if(state == NOT_RUNNING) {
		repeat_counter = 0;
		startWithDelay(true);
	}
	return *this;
}

poTween& poTween::stop(bool jump) {
	if(state == RUNNING) {
		state = NOT_RUNNING;
		if(jump)
			*value = end_value;
	}
	return *this;
}

bool poTween::isRunning() const {
	return state == RUNNING;
}

bool poTween::isComplete() const {
	return state == COMPLETE;
}

void poTween::startWithDelay(bool do_delay) {
	state = RUNNING;
	last_time = nowTime();
	time = 0;

	if(do_delay) {
		begin = delay;
		end = begin + duration;
	}
	else {
		begin = 0;
		end = duration;
	}

}

poTween& poTween::tween(poTweenMajorType major, poTweenMinorType minor, 
						float end_val, float dur, float del)
{
	begin_value = *value;
	end_value = end_val;
	duration = dur;
	delay = del;
	tween_func = tweenFuncTable[major][minor];
	return *this;
}



