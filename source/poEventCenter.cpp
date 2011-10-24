//
//  poEventCenter.cpp
//  potionCode
//
//  Created by Jared Schiffman on 10/16/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poEventCenter.h"
#include "poObject.h"
#include "poApplication.h"



void localizeEvent(poEvent &global_event, poEvent &tolocal) {
	tolocal.timestamp			= global_event.timestamp;
	tolocal.modifiers			= global_event.modifiers;
    tolocal.globalPosition		= global_event.globalPosition;
	tolocal.previousPosition	= global_event.previousPosition;
	tolocal.keyChar				= global_event.keyChar;
	tolocal.keyCode				= global_event.keyCode;
	tolocal.touchID				= global_event.touchID;
	
	if(isMouseEvent(global_event.type) || isTouchEvent(global_event.type)) {
		// flip the coords so the local position can match the orientation of the global one
		poPoint pt = tolocal.source->globalToLocal(poPoint(tolocal.position.x, getWindowHeight()-tolocal.position.y));
		tolocal.position = pt;
	}
}



poEventCenter *poEventCenter::get() {
	static poEventCenter *instance = NULL;
	if(!instance)
		instance = new poEventCenter();
	return instance;
}

poEventCenter::poEventCenter() 
:	events(PO_LAST_EVENT)
{}

void poEventCenter::addEvent(int eventType, poObject *source, std::string message, const poDictionary& dict) {
	return addEvent(eventType, source, source, message, dict);
}

void poEventCenter::addEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict) {
	poEventCallback* callback = new poEventCallback();
	callback->receiver = sink;
	callback->event = poEvent(eventType, source, message, dict);
	events[eventType].push_back(callback);
}

void poEventCenter::removeAllEvents(poObject* obj) {
	BOOST_FOREACH(std::vector<poEventCallback*> &event_vec, events) {
        for(int i=event_vec.size()-1; i>=0; i--) {
            if(event_vec[i]->event.source == obj || event_vec[i]->receiver == obj) {
				delete event_vec[i];
                event_vec.erase(event_vec.begin()+i);
            }
        }
	}
}

void poEventCenter::removeAllEventsOfType(poObject* obj, int eventType) {
	std::vector<poEventCallback*> &event_vec = events[eventType];
	for(int i=event_vec.size()-1; i>=0; i--) {
		if(event_vec[i]->event.source == obj || event_vec[i]->receiver == obj) {
			delete event_vec[i];
			event_vec.erase(event_vec.begin()+i);
		}
	}
}

bool poEventCenter::objectHasEvent(poObject *obj, int eventType) {
	std::vector<poEventCallback*> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i]->event.source)
			return true;
	}
	return false;
}

void poEventCenter::copyEventsFromObject(poObject *from, poObject *to) {
	for(int i=0; i<events.size(); i++) {
		std::vector<poEventCallback*> additions;
		
		for(int j=0; j<events[i].size(); j++) {
			if(events[i][j]->receiver == from) {
				poEventCallback *cb = new poEventCallback;
				cb->receiver = to;
				cb->event = events[i][j]->event;
				additions.push_back(cb);
			}
			else if(events[i][j]->event.source == from) {
				poEventCallback *cb = new poEventCallback;
				cb->event = events[i][j]->event;
				cb->event.source = to;
				additions.push_back(cb);
			}
		}
		
		events[i].insert(events[i].end(), additions.begin(), additions.end());
	}
}

// ======================= EVENT REWRITE CODE ======================= 


void poEventCenter::notifyAllListeners( poEvent global_event )
{
    // for all registed event listeners
	std::vector<poEventCallback*> &event_vec = events[global_event.type];
	for(int i=0; i<event_vec.size(); i++)
        notifyOneListener( event_vec[i], global_event );
}

void poEventCenter::notifyOneListener( poEventCallback* callback, poEvent global_event )
{
    // get event stored in callback
    poEvent &stored_event = callback->event;
	poEvent sentEvent = stored_event;
	
    if(stored_event.source->visible)
    {
        // prep the event for sending
        localizeEvent(global_event, sentEvent);
		
        // push it out
        callback->receiver->eventHandler(&sentEvent);
        
        // capture any user changes to the dictionary
        stored_event.dictionary = sentEvent.dictionary;
    }
}

poEventCallback* poEventCenter::findTopObjectUnderPoint( int eventType, poPoint P )
{
    std::vector<poEventCallback*> &event_vec = events[eventType];
	
    // for all registed event listeners
    for( int i=event_vec.size()-1; i>=0; i-- )
    {
        poObject* obj = event_vec[i]->event.source;
        
        // check that object is visible
        if( !obj->visible || obj->alpha < 0.01 || obj->drawOrder() == -1 )
            continue; 
		
        // if point is inside, return callback containing object
        if ( obj->pointInside( P, true ) )
        {
            return event_vec[i];
        }
    }
    
    // if no object found, return NULL
    return NULL;
}

void poEventCenter::processEvents(std::deque<poEvent> &events) {
    // sort all event callbacks by draw order
    sortCallbacksByDrawOrder();
    
	while(!events.empty()) {
        //Get Event
		poEvent &event = events.front();
        
        if(isMouseEvent(event.type)) {
            processMouseEvents(event);
        }
        
        else if(isTouchEvent(event.type)) {
			processTouchEvents(event);
        }
        
        else if(isKeyEvent(event.type)) {
			processKeyEvents(event);
        }
        
        else {
            std::cout << "Unknown event type! (" << event.type << ")" << std::endl;
        }
		
		events.pop_front();
    }
}

void poEventCenter::processMouseEvents( poEvent &Event )
{
    // handles PO_MOUSE_DOWN_EVENT and PO_MOUSE_DOWN_INSIDE_EVENT
    if ( Event.type == PO_MOUSE_DOWN_EVENT )
    {
        // notify all objects listening for PO_MOUSE_DOWN_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );
		
        // find single object to receive PO_MOUSE_DOWN_INSIDE_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_DOWN_INSIDE_EVENT;
        poEventCallback* callback = findTopObjectUnderPoint( PO_MOUSE_DOWN_INSIDE_EVENT, Event.position );
        if ( callback )
            notifyOneListener( callback, sentEvent );
    }
    
    // handles PO_MOUSE_UP_EVENT
    // also handles last part of PO_MOUSE_DRAG_EVENT
    if ( Event.type == PO_MOUSE_UP_EVENT )
    {
        // notify all objects listening for PO_MOUSE_UP_EVERYWHERE_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );
        
        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_DRAG_EVENT];
        for( int i=0; i<event_vec.size(); i++ )
        {
            poEventCallback* callback = event_vec[i];
            poObject* obj = callback->event.source;
            obj->eventMemory.lastDragID = -1;
        }
    }
    
    // handles PO_MOUSE_MOVE_EVENT, PO_MOUSE_OVER_EVENT, PO_MOUSE_ENTER_EVENT and PO_MOUSE_LEAVE_EVENT
    if ( Event.type == PO_MOUSE_MOVE_EVENT )
    {
        // notify all objects listening for PO_MOUSE_MOVE_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );
        
        // for all objects listening for PO_MOUSE_ENTER_EVENT and PO_MOUSE_LEAVE_EVENT
        // need to combine ENTER and LEAVE into one stack
        // right now just listens to ENTER stack and sends LEAVE as well
        sentEvent = Event;
        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_ENTER_EVENT];
        for( int i=0; i<event_vec.size(); i++ )
        {
            poEventCallback* callback = event_vec[i];
            poObject* obj = callback->event.source;
            // check that object is visible
            if( !obj->visible || obj->alpha < 0.01 || obj->drawOrder() == -1 )
                continue; 
			
            // if point is inside
            if ( obj->pointInside( Event.position, true ) )
            {
                // if lastInsideTouchID is -1 (nothing inside before), then notify ENTER
                if ( obj->eventMemory.lastInsideTouchID == -1 )
                {
                    sentEvent.type = PO_MOUSE_ENTER_EVENT;
                    notifyOneListener( callback, sentEvent );
                }
                obj->eventMemory.lastInsideTouchID = 1;
            }
            else
            {
                // if lastInsideTouchID is not -1 (nothing inside before), then notify LEAVE
                if ( obj->eventMemory.lastInsideTouchID != -1 )
                {
                    sentEvent.type = PO_MOUSE_LEAVE_EVENT;
                    notifyOneListener( callback, sentEvent );
                }
                // not inside, set lastInsideTouchID to -1
                obj->eventMemory.lastInsideTouchID = -1;
            }
        }
    }
    
    // handles PO_MOUSE_DRAG_EVERYWHERE_EVENT and PO_MOUSE_DRAG_EVENT
    if ( Event.type == PO_MOUSE_DRAG_EVENT )
    {
        // notify all objects listening for PO_MOUSE_DRAG_EVERYWHERE_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );
        
        // for all objects listening to PO_MOUSE_DRAG_EVENT with lastDragID set
        // for mouse control, this should just be a single object in the set
        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_DRAG_EVENT];
        for( int i=0; i<event_vec.size(); i++ )
        {
            poEventCallback* callback = event_vec[i];
            poObject* obj = callback->event.source;
            if ( obj->eventMemory.lastDragID != -1 )
            {  
                sentEvent.type = PO_MOUSE_DRAG_EVENT;
                notifyOneListener( callback, sentEvent );
            }
        }
    }
}


void    poEventCenter::processTouchEvents( poEvent &Event )
{
	/*
	 // First notify all objects listening for ..._EVERYWHERE_EVENTs
	 poEvent sentEvent = Event;
	 notifyAllListeners( sentEvent );
	 
	 // handles PO_MOUSE_DOWN_EVERYWHERE_EVENT, PO_MOUSE_DOWN_INSIDE_EVENT and PO_MOUSE_DOWN_OUTSIDE_EVENT
	 if ( Event.type == PO_TOUCH_BEGAN_EVERYWHERE_EVENT )
	 {
	 // find single object to receive PO_MOUSE_DOWN_INSIDE_EVENT
	 sentEvent = Event;
	 sentEvent.type = PO_TOUCH_BEGAN_INSIDE_EVENT;
	 
	 poEventCallback* callback = findTopObjectUnderPoint( PO_TOUCH_BEGAN_INSIDE_EVENT, Event.position );
	 if ( callback )
	 notifyOneListener( callback, sentEvent );
	 
	 // notify objects listening for PO_MOUSE_DOWN_OUTSIDE_EVENT
	 sentEvent = Event;
	 sentEvent.type = PO_TOUCH_BEGAN_OUTSIDE_EVENT;
	 std::vector<poEventCallback*> &event_vec = events[PO_TOUCH_BEGAN_OUTSIDE_EVENT];
	 for( int i=0; i<event_vec.size(); i++ )
	 {
	 poEventCallback* callback = event_vec[i];
	 poObject* obj = callback->event.source;
	 // check that object is visible
	 if( !obj->visible || obj->alpha < 0.01 || obj->drawOrder() == -1 )
	 continue; 
	 // if point is not inside, send event
	 if ( ! obj->pointInside( Event.position, true ) )
	 notifyOneListener( callback, sentEvent );
	 }
	 }
	 
	 
	 // handles PO_MOUSE_UP_EVERYWHERE_EVENT, PO_MOUSE_UP_INSIDE_EVENT and PO_MOUSE_UP_OUTSIDE_EVENT
	 // also handles last part of PO_MOUSE_DRAG_EVENT
	 if ( Event.type == PO_TOUCH_ENDED_EVERYWHERE_EVENT )
	 {
	 // find single object to receive PO_MOUSE_UP_INSIDE_EVENT
	 sentEvent.type = PO_TOUCH_ENDED_INSIDE_EVENT;
	 
	 poEventCallback* callback = findTopObjectUnderPoint( PO_TOUCH_ENDED_INSIDE_EVENT, Event.position );
	 if ( callback )
	 notifyOneListener( callback, sentEvent );
	 
	 // notify objects listening for PO_MOUSE_UP_OUTSIDE_EVENT
	 sentEvent = Event;
	 sentEvent.type = PO_TOUCH_ENDED_OUTSIDE_EVENT;
	 std::vector<poEventCallback*> &event_vec = events[PO_TOUCH_ENDED_OUTSIDE_EVENT];
	 for( int i=0; i<event_vec.size(); i++ )
	 {
	 poEventCallback* callback = event_vec[i];
	 poObject* obj = callback->event.source;
	 // check that object is visible
	 if( !obj->visible || obj->alpha < 0.01 || obj->drawOrder() == -1 )
	 continue; 
	 // if point is not inside, send event
	 if ( ! obj->pointInside( Event.position, true ) )
	 notifyOneListener( callback, sentEvent );
	 }
	 }
	 
	 
	 // handles PO_MOUSE_MOVE_EVENT, PO_MOUSE_OVER_EVENT, PO_MOUSE_ENTER_EVENT and PO_MOUSE_LEAVE_EVENT
	 //    if ( Event.type == PO_TOUCH_MOVED_EVERYWHERE_EVENT )
	 //    {
	 //        
	 //        // find single object to receive PO_MOUSE_OVER_EVENT
	 //        sentEvent.type = PO_MOUSE_OVER_EVENT;
	 //        poEventCallback* callback = findTopObjectUnderPoint( PO_MOUSE_OVER_EVENT, Event.position );
	 //        if ( callback )
	 //            notifyOneListener( callback, sentEvent );
	 //        
	 //        // for all objects listening for PO_MOUSE_ENTER_EVENT and PO_MOUSE_LEAVE_EVENT
	 //        // need to combine ENTER and LEAVE into one stack
	 //        // right now just listens to ENTER stack and sends LEAVE as well
	 //        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_ENTER_EVENT];
	 //        for( int i=0; i<event_vec.size(); i++ )
	 //        {
	 //            poEventCallback* callback = event_vec[i];
	 //            poObject* obj = callback->event.source;
	 //            // check that object is visible
	 //            if( !obj->visible || obj->alpha < 0.01 || obj->drawOrder() == -1 )
	 //                continue; 
	 //            // if point is inside
	 //            if ( obj->pointInside( Event.position, true ) )
	 //            {
	 //                // if lastInsideTouchID is -1 (nothing inside before), then notify ENTER
	 //                if ( obj->eventMemory.lastInsideTouchID == -1 )
	 //                {
	 //                    sentEvent.type = PO_MOUSE_ENTER_EVENT;
	 //                    notifyOneListener( callback, sentEvent );
	 //                }
	 //                obj->eventMemory.lastInsideTouchID = 1;
	 //            }
	 //            else
	 //            {
	 //                // if lastInsideTouchID is not -1 (nothing inside before), then notify LEAVE
	 //                if ( obj->eventMemory.lastInsideTouchID != -1 )
	 //                {
	 //                    sentEvent.type = PO_MOUSE_LEAVE_EVENT;
	 //                    notifyOneListener( callback, sentEvent );
	 //                }
	 //                // not inside, set lastInsideTouchID to -1
	 //                obj->eventMemory.lastInsideTouchID = -1;
	 //            }
	 //        }
	 //    }
	 */
}


void poEventCenter::processKeyEvents( poEvent &Event ) {
	notifyAllListeners(Event);
}

bool sortCallbacksByDrawOrderFunc(poEventCallback* a, poEventCallback* b) {
	return a->event.source->drawOrder() < b->event.source->drawOrder();
}

void poEventCenter::sortCallbacksByDrawOrder() {
    // sort only event callback vectors where order matters
    std::sort( events[PO_MOUSE_DOWN_INSIDE_EVENT].begin(), events[PO_MOUSE_DOWN_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );
}

std::vector<poEvent*> poEventCenter::eventsForObject(poObject *obj, int eventType) {
	std::vector<poEvent*> response;
	
	std::vector<poEventCallback*> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i]->event.source)
			response.push_back(&(event_vec[i]->event));
	}
	return response;
}




