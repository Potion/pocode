/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poTween.h"
#include "poObject.h"
#include "poHelpers.h"

#undef min
#undef max

namespace po {
    TweenBase::TweenBase()
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

    TweenBase::~TweenBase() {}

    void TweenBase::update() {
        if(!isRunning())
            return;
        
        if(tweenFunc) {
            float now = po::getElapsedTime();
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
                    TweenFinishedCallback cb = callback;
                    TweenFinishedCallback().swap(callback);
                    cb();
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setRepeat(TweenRepeat type, int count) {
        repeatType = type;
        repeatCount = count;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setNotification(Object *obj, const std::string &msg, const Dictionary &d) {
        if(obj) {
            Dictionary dict = d;
            dict.set("tween", this);
            callback = boost::bind(&Object::messageHandler, obj, msg, dict);
        }
        else {
            TweenFinishedCallback().swap(callback);
        }
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setTweenFunction(TweenFunction func) {
        tweenFunc = func;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setDuration(float dur) {
        duration = dur;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setDelay(float del, bool d) {
        delay = del;
        delay_on_repeat = d;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::setExtraValues(float e1, float e2) {
        extra1 = e1;
        extra2 = e2;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::start() {
        if(state != RUNNING) {
            repeatCounter = 0;
            startWithDelay(true);
        }
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    TweenBase& TweenBase::stop(bool jumpToEnd) {
        if(state == RUNNING) {
            state = NOT_RUNNING;
            
            if(jumpToEnd)
                setValueToEnd();

            TweenFinishedCallback().swap(callback);
        }
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    bool TweenBase::isRunning() const {
        return state == RUNNING;
    }
    
    
    //------------------------------------------------------------------------
    bool TweenBase::isComplete() const {
        return state == COMPLETE;
    }
    
    
    //------------------------------------------------------------------------
    bool TweenBase::isRepeating() const {
        return repeatType != PO_TWEEN_REPEAT_NONE;
    }
    
    
    //------------------------------------------------------------------------
    int TweenBase::getRepeatCount() const {
        return repeatCounter;
    }
    
    
    //------------------------------------------------------------------------
    float TweenBase::getProgress() const {
        return (float)std::min(1.f, std::max(0.f, (time - beginTime - delay) / duration));
    }
    
    
    //------------------------------------------------------------------------
    void TweenBase::reset() {
        state = NOT_RUNNING;
    }
    
    
    //------------------------------------------------------------------------
    void TweenBase::startWithDelay(bool doDelay) {
        state = RUNNING;
        
        lastTime = po::getElapsedTime();
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
}/*End po namespace */