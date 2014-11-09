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

//
//  EventCenter.cpp
//  pocode
//
//  Created by Jared Schiffman on 10/16/11.
//  Copyright 2011 Potion Design. All rights reserved.
//
#include "poEventCenter.h"
#include "poObject.h"
#include "poApplication.h"

#include <vector>
#include <iostream>
#include <boost/foreach.hpp>

namespace po {
    void localizeEvent(Event &global_event, Event &tolocal) {
        tolocal.timestamp			= global_event.timestamp;
        tolocal.modifiers			= global_event.modifiers;
        tolocal.globalPosition		= global_event.globalPosition;
        tolocal.previousPosition	= global_event.previousPosition;
        tolocal.keyChar				= global_event.keyChar;
        tolocal.keyCode				= global_event.keyCode;
        tolocal.touchID				= global_event.touchID;
        tolocal.uniqueID			= global_event.uniqueID;
        tolocal.motion              = global_event.motion;
        
        if(isMouseEvent(global_event.type) || isTouchEvent(global_event.type)) {
            // flip the coords so the local position can match the orientation of the global one
            Point pt = tolocal.source->globalToLocal(Point(tolocal.globalPosition.x, tolocal.globalPosition.y));
            tolocal.localPosition = pt;
            tolocal.localPosition.z = 0; // z = 1 out of globalToLocal math
        }
    }
    
    
    //------------------------------------------------------------------------
    EventCenter *EventCenter::get() {
        static EventCenter *instance = NULL;
        if(!instance)
            instance = new EventCenter();
        return instance;
    }
    
    
    //------------------------------------------------------------------------
    EventCenter::EventCenter() 
    :	events(po::LAST_EVENT)
    {}
    
    
    //------------------------------------------------------------------------
    void EventCenter::addEvent(int eventType, Object *source, std::string message, const Dictionary& dict) {
        return addEvent(eventType, source, source, message, dict);
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::addEvent(int eventType, Object *source, Object *sink, std::string message, const Dictionary& dict) {
        EventCallback* callback = new EventCallback();
        // make sure the source has a slot for event info
        if(!source->eventMemory)
            source->eventMemory = new EventMemory;
        callback->receiver = sink;
        callback->event = Event(eventType, source, message, dict);
        events[eventType].push_back(callback);
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::removeAllEvents(Object* obj) {
        // check the regular event queues
        BOOST_FOREACH(std::vector<EventCallback*> &event_vec, events) {
            for(int i=(int)event_vec.size()-1; i>=0; i--) {
                if(event_vec[i]->event.source == obj || event_vec[i]->receiver == obj) {
                    // remove the memory slot
                    delete event_vec[i]->event.source->eventMemory;
                    event_vec[i]->event.source->eventMemory = NULL;
                    
                    // and remove the event
                    delete event_vec[i];
                    event_vec.erase(event_vec.begin()+i);
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::removeAllEventsOfType(Object* obj, int eventType) {
        std::vector<EventCallback*> &event_vec = events[eventType];
        size_t size = event_vec.size();
        if (size > 0)
        {
            for(size_t i=event_vec.size()-1; i!=0; i--) {
                if(event_vec[i]->event.source == obj || event_vec[i]->receiver == obj) {
                    // remove the memory slot
                    delete event_vec[i]->event.source->eventMemory;
                    event_vec[i]->event.source->eventMemory = NULL;

                    // and remove the event
                    delete event_vec[i];
                    event_vec.erase(event_vec.begin()+i);
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    bool EventCenter::objectHasEvent(Object *obj, int eventType) {
        std::vector<EventCallback*> &event_vec = events[eventType];
        for(int i=0; i<event_vec.size(); i++) {
            if(obj == event_vec[i]->event.source)
                return true;
        }
        return false;
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::copyEventsFromObject(Object *from, Object *to) {
        for(int i=0; i<events.size(); i++) {
            std::vector<EventCallback*> additions;
            
            for(int j=0; j<events[i].size(); j++) {
                if(events[i][j]->receiver == from) {
                    EventCallback *cb = new EventCallback;
                    cb->receiver = to;
                    cb->event = events[i][j]->event;
                    additions.push_back(cb);
                }
                else if(events[i][j]->event.source == from) {
                    EventCallback *cb = new EventCallback;
                    cb->event = events[i][j]->event;
                    cb->event.source = to;
                    additions.push_back(cb);
                }
            }
            
            events[i].insert(events[i].end(), additions.begin(), additions.end());
        }
    }
    
    
    //------------------------------------------------------------------------
    // ======================= EVENT REWRITE CODE ======================= 

    
    
    
    //------------------------------------------------------------------------
    void EventCenter::notifyAllListeners( Event &global_event ) {    
        // for all registed event listeners
        std::vector<EventCallback*> &event_vec = events[global_event.type];
        for(int i=0; i<event_vec.size(); i++)
                notifyOneListener( event_vec[i], global_event );
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::notifyOneListener( EventCallback* callback, Event &global_event ) {
        if(!callback->bShouldDelete) {
            // get event stored in callback
            Event &stored_event = callback->event;
            Event sentEvent = stored_event;
            
            if(objectIsAvailableForEvents(stored_event.source)) {
                // prep the event for sending
                localizeEvent(global_event, sentEvent);
                
                // push it out
                callback->receiver->eventHandler(&sentEvent);
                
                // capture any user changes to the dictionary
                stored_event.dictionary = sentEvent.dictionary;
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    EventCallback* EventCenter::findTopObjectUnderPoint( int eventType, Point P ) {
        std::vector<EventCallback*> &event_vec = events[eventType];
        
        // for all registed event listeners
        size_t size = event_vec.size();
        if (size > 0)
        {
            for( size_t i=event_vec.size()-1; i!=0; i-- ) {
                Object* obj = event_vec[i]->event.source;

                // check that object is visible
                if( objectIsAvailableForEvents(obj) ) {
                    // if point is inside, return callback containing object
                    if ( obj->pointInside( P, true ) )
                        if(!event_vec[i]->bShouldDelete)
                            return event_vec[i];
                }
            }
        }
        
        // if no object found, return NULL
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::processEvents(std::deque<Event> &events) {
        // sort all event callbacks by draw order
        sortCallbacksByDrawOrder();

        while(!events.empty()) {
            //Get Event
            Event &event = events.front();
            
            if(isMouseEvent(event.type)) {
                processMouseEvents(event);
            }
            
            else if(isTouchEvent(event.type)) {
                processTouchEvents(event);
            }
            
            else if(isKeyEvent(event.type)) {
                processKeyEvents(event);
            }
            
            else if(isMotionEvent(event.type)) {
                processMotionEvent(event);
            }
            
            else {
                std::cout << "Unknown event type! (" << event.type << ")" << std::endl;
            }
            
            events.pop_front();
        }
        
        cleanupCallbacks();
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::cleanupCallbacks() {
        std::vector<std::vector<EventCallback *> >::iterator iter = events.begin(); //Events Iterator
        for(; iter != events.end(); ++iter) {
            std::vector<EventCallback *>::iterator cIter = (*iter).begin(); //CallBack iterator for each event
            for(; cIter != (*iter).end(); ++cIter) {
                if((*cIter)->bShouldDelete) {
                    cIter = (*iter).erase(cIter);
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::processMouseEvents( Event &Event ) {

        // notify all objects listening for basic events
        notifyAllListeners( Event );
        
        
        // handles po::MOUSE_DOWN_EVENT and po::MOUSE_DOWN_INSIDE_EVENT
        if ( Event.type == po::MOUSE_DOWN_EVENT ) {
      
            // find single object to receive po::MOUSE_DOWN_INSIDE_EVENT
            
            EventCallback* callback = findTopObjectUnderPoint( po::MOUSE_DOWN_INSIDE_EVENT, Event.globalPosition );
            if (callback)
            {
                Event.type = po::MOUSE_DOWN_INSIDE_EVENT;
                notifyOneListener( callback, Event );
            }
            
            // set lastDragID for object to receive po::MOUSE_DRAG_INSIDE_EVENT
            EventCallback* drag_callback = findTopObjectUnderPoint( po::MOUSE_DRAG_INSIDE_EVENT, Event.globalPosition );
            if (drag_callback)
            {
                Object* obj = drag_callback->event.source;
                obj->eventMemory->lastDragID = 1;
            }
        }
        
        // handles po::MOUSE_UP_EVENT
        // also handles last part of po::MOUSE_DRAG_EVENT
        else if ( Event.type == po::MOUSE_UP_EVENT ) {
            
            // reset lastDragID for object receiving po::MOUSE_DRAG_EVENT
            std::vector<EventCallback*> &event_vec = events[po::MOUSE_DRAG_INSIDE_EVENT];
            for( int i=0; i<event_vec.size(); i++ ) {
                EventCallback* callback = event_vec[i];
                if(!callback->bShouldDelete) {
                    Object* obj = callback->event.source;
                    obj->eventMemory->lastDragID = -1;
                }
            }
        }
        
        // handles po::MOUSE_MOVE_EVENT, po::MOUSE_OVER_EVENT, po::MOUSE_ENTER_EVENT and po::MOUSE_LEAVE_EVENT
        else if ( Event.type == po::MOUSE_MOVE_EVENT ) {

            // move over events
            EventCallback* callback = findTopObjectUnderPoint( po::MOUSE_OVER_EVENT, Event.globalPosition );
            if ( callback)
            {
                Event.type = po::MOUSE_OVER_EVENT;
                notifyOneListener( callback, Event );
                Event.type = po::MOUSE_MOVE_EVENT;
            }
            
            // lastInsideTouchID states
            // -1 = last touch outside (initialized to this)
            // -2 = last touch outside, this touch inside, will be reset to -1
            //  1 = last touch inside
            
            // enter events
            std::vector<EventCallback*> &enter_event_vec = events[po::MOUSE_ENTER_EVENT];
            for( int i=0; i<enter_event_vec.size(); i++ ) {
                EventCallback* callback = enter_event_vec[i];
                if(!callback->bShouldDelete) {
                    Object* obj = callback->event.source;
                    
                    if( ! objectIsAvailableForEvents(obj) )
                        continue;
                    
                    bool isInside = obj->pointInside(Event.globalPosition, true);
                    
                    // if inside now and not inside last frame, then trigger enter event
                    if(isInside && obj->eventMemory->lastInsideTouchID < 0) 
                    {
                        notifyOneListener(callback, Event);
                        obj->eventMemory->lastInsideTouchID = -2;       // -2 will become positive 1 in for loop after leave event
                    }
                    if (!isInside && obj->eventMemory->lastInsideTouchID > 0)
                        obj->eventMemory->lastInsideTouchID = 2;       // 2 will become  -1 in for loop after leave event
                }
            }
            
            // leave events
            std::vector<EventCallback*> &leave_event_vec = events[po::MOUSE_LEAVE_EVENT];
            for( int i=0; i<leave_event_vec.size(); i++ ) {
                EventCallback* callback = leave_event_vec[i];
                if(!callback->bShouldDelete) {
                    Object* obj = callback->event.source;
                    
                    if( ! objectIsAvailableForEvents(obj) )
                        continue;
                    
                    bool isInside = obj->pointInside(Event.globalPosition, true);
                    
                    // if not inside now and was inside last frame, then trigger leave event
                    if( !isInside && obj->eventMemory->lastInsideTouchID > 0 ) 
                    {
                        notifyOneListener(callback, Event);
                        obj->eventMemory->lastInsideTouchID = -1;
                    }
                    if ( isInside && obj->eventMemory->lastInsideTouchID < 0 )
                        obj->eventMemory->lastInsideTouchID = 1;
                }
            }
            
            // go back and fix states for enter events
            for( int i=0; i<enter_event_vec.size(); i++ )
            {
                EventCallback* callback = enter_event_vec[i];
                if(!callback->bShouldDelete) {
                    Object* obj = callback->event.source;
                    // for all enter events, switch lastInsideTouchID from -2 to to 1, and  2 to -1
                    if ( obj->eventMemory->lastInsideTouchID == -2 )
                        obj->eventMemory->lastInsideTouchID = 1;
                    if ( obj->eventMemory->lastInsideTouchID == 2 )
                        obj->eventMemory->lastInsideTouchID = -1;
                }
            }		
        }
        
        // handles po::MOUSE_DRAG_EVERYWHERE_EVENT and po::MOUSE_DRAG_EVENT
        else if ( Event.type == po::MOUSE_DRAG_EVENT ) {

            // for all objects listening to po::MOUSE_DRAG_INSIDE_EVENT with lastDragID set
            // for mouse control, this should just be a single object in the set
            std::vector<EventCallback*> &event_vec = events[po::MOUSE_DRAG_INSIDE_EVENT];
            for( int i=0; i<event_vec.size(); i++ ) {
                EventCallback* callback = event_vec[i];
                if(!callback->bShouldDelete) {
                Object* obj = callback->event.source;
                    if ( obj->eventMemory->lastDragID != -1 ) {  
                        Event.type = po::MOUSE_DRAG_INSIDE_EVENT;
                        notifyOneListener( callback, Event );
                    }
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::processTouchEvents( Event &Event ) {
         // First notify all objects listening for ..._EVERYWHERE_EVENTs
         notifyAllListeners( Event );
         
         // handles po::TOUCH_BEGAN_INSIDE_EVENT and po::TOUCH_BEGAN_OUTSIDE_EVENT
         if ( Event.type == po::TOUCH_BEGAN_EVENT ) {
             EventCallback* callback = findTopObjectUnderPoint( po::TOUCH_BEGAN_INSIDE_EVENT, Event.globalPosition );
             if (callback) {
                 Event.type = po::TOUCH_BEGAN_INSIDE_EVENT;
                 notifyOneListener( callback, Event );
             }
             
             // notify objects listening for po::TOUCH_BEGAN_OUTSIDE_EVENT
             Event.type = po::TOUCH_BEGAN_OUTSIDE_EVENT;
             std::vector<EventCallback*> &event_vec = events[po::TOUCH_BEGAN_OUTSIDE_EVENT];
             for( int i=0; i<event_vec.size(); i++ ) {
                 EventCallback* callback = event_vec[i];
                 if(!callback->bShouldDelete) {
                     Object* obj = callback->event.source;
                     // check that object is visible
                     if( objectIsAvailableForEvents(obj) ) {
                         // if point is not inside, send event
                         if ( ! obj->pointInside( Event.globalPosition, true ) )
                             notifyOneListener( callback, Event );
                     }
                 }
             }
         }
         
         // handles po::TOUCH_ENDED_EVENT, po::MOUSE_UP_INSIDE_EVENT and po::TOUCH_ENDED_OUTSIDE_EVENT
         // also handles last part of po::MOUSE_DRAG_EVENT
        else if ( Event.type == po::TOUCH_ENDED_EVENT ) {
            // find single object to receive po::MOUSE_UP_INSIDE_EVENT
            Event.type = po::TOUCH_ENDED_INSIDE_EVENT;
            
            EventCallback* callback = findTopObjectUnderPoint( po::TOUCH_ENDED_INSIDE_EVENT, Event.globalPosition );
            if (callback)
                notifyOneListener( callback, Event );
            
            // notify objects listening for po::TOUCH_ENDED_OUTSIDE_EVENT
            Event.type = po::TOUCH_ENDED_OUTSIDE_EVENT;
            std::vector<EventCallback*> &event_vec = events[po::TOUCH_ENDED_OUTSIDE_EVENT];
            for( int i=0; i<event_vec.size(); i++ ) {
                EventCallback* callback = event_vec[i];
                if(!callback->bShouldDelete) {
                    Object* obj = callback->event.source;
                    // check that object is visible
                    if( objectIsAvailableForEvents(obj) ) {
                        // if point is not inside, send event
                        if ( ! obj->pointInside( Event.globalPosition, true ) )
                            notifyOneListener( callback, Event );
                    }
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::processMotionEvent( Event &Event ) {
        notifyAllListeners(Event);
    }
    
    
    //------------------------------------------------------------------------
    void processEnterLeave(std::deque<EventCallback*> &e) {
        
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::processKeyEvents( Event &Event ) {
        notifyAllListeners(Event);
    }
    
    
    //------------------------------------------------------------------------
    bool sortCallbacksByDrawOrderFunc(EventCallback* a, EventCallback* b) {
        return a->event.source->getDrawOrder() < b->event.source->getDrawOrder();
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::sortCallbacksByDrawOrder() {
        // sort only event callback vectors where order matters
        std::sort( events[po::MOUSE_DOWN_INSIDE_EVENT].begin(), events[po::MOUSE_DOWN_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );
        std::sort( events[po::MOUSE_OVER_EVENT].begin(), events[po::MOUSE_OVER_EVENT].end(), sortCallbacksByDrawOrderFunc );
        std::sort( events[po::TOUCH_BEGAN_INSIDE_EVENT].begin(), events[po::TOUCH_BEGAN_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );
        std::sort( events[po::TOUCH_ENDED_INSIDE_EVENT].begin(), events[po::TOUCH_ENDED_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );
    }
    
    
    //------------------------------------------------------------------------
    std::vector<Event*> EventCenter::eventsForObject(Object *obj, int eventType) {
        std::vector<Event*> response;
        
        std::vector<EventCallback*> &event_vec = events[eventType];
        for(int i=0; i<event_vec.size(); i++) {
            if(obj == event_vec[i]->event.source)
                response.push_back(&(event_vec[i]->event));
        }
        
        return response;
    }
    
    
    //------------------------------------------------------------------------
    void EventCenter::negateDrawOrderForObjectWithEvents() {
        for(auto &event : events) {
        //for(int i=0; i<events.size(); i++) {
            for(auto &eventCallback : event) {
                //for(int j=0; j<events[i].size(); j++) {
                eventCallback->event.source->drawOrder = -1;
//                events[i][j]->event.source->drawOrder = -1;
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    bool EventCenter::objectIsAvailableForEvents(Object *obj) {
        bool isAvailable = (obj->visible && obj->getDrawOrder() != -1);
        
        if(isAvailable && po::applicationGetWindow()->getUseAlphaForEvents()) {
            isAvailable = obj->alpha > 0.01;
        }
        
        return isAvailable;
        
    }
} /*End po namespace */
