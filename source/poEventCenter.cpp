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
    tolocal.position       = global_event.position;
    
	tolocal.keyChar            = global_event.keyChar;
	tolocal.keyCode            = global_event.keyCode;
	tolocal.modifiers          = global_event.modifiers;
	tolocal.uid                = global_event.uid;
	tolocal.timestamp          = global_event.timestamp;
	tolocal.previous_position  = global_event.previous_position;
	tolocal.touchID            = global_event.touchID;
	
	if(isMouseEvent(global_event.type) || isTouchEvent(global_event.type)) {
		// flip the coords so the local position can match the orientation of the global one
		poPoint pt = tolocal.source->globalToLocal(poPoint(tolocal.position.x, getWindowHeight()-tolocal.position.y));
		tolocal.local_position = pt;
	}
}



poEventCenter *poEventCenter::get() {
	static poEventCenter *instance = NULL;
	if(!instance)
		instance = new poEventCenter();
	return instance;
}

void poEventCenter::resizeIfNecessary(int incoming) {
	if(incoming >= events.size()) {
		events.resize(incoming);
		bcheck_event.resize(incoming);
	}
}

poEventCenter::poEventCenter() 
:	events(PO_LAST_EVENT)
,	bcheck_event(PO_LAST_EVENT)
{
}

int poEventCenter::registerForEvent(int eventType, poObject *source, std::string message, const poDictionary& dict) {
	return registerForEvent(eventType, source, source, message, dict);
}

int poEventCenter::registerForEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict) {
	if(eventType >= events.size())
		return 0;
    
	static int callback_uid = 0;
	
	poEventCallback* callback = new poEventCallback();
	callback->uid = ++callback_uid;
	callback->receiver = sink;
	callback->event = poEvent(eventType, source, message, dict);
	events[eventType].push_back(callback);
    
	return callback->uid;
}

void poEventCenter::removeEvent(int uid) {
	BOOST_FOREACH(std::vector<poEventCallback*> &event_vec, events) {
		std::vector<poEventCallback*>::iterator iter = event_vec.begin();
		while(iter != event_vec.end()) {
			if((*iter)->uid == uid)
				iter = event_vec.erase(iter);
			else
				iter++;
		}
	}
}

void poEventCenter::removeAllEvents(poObject* obj) {
	BOOST_FOREACH(std::vector<poEventCallback*> &event_vec, events) {
        for(int i=event_vec.size()-1; i>=0; i--) {
            if(event_vec[i]->event.source == obj || event_vec[i]->receiver == obj) {
                event_vec.erase(event_vec.begin()+i);
            }
        }
	}
}

void poEventCenter::notify(poEvent event) {
	if(event.type >= events.size())
		return;
	
	std::vector<poEventCallback*> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		poEventCallback* callback = event_vec[i];
		poEvent &stored_event = callback->event;
		if(stored_event.source->visible) {
			// prep the event for sending
			localizeEvent(stored_event, event);
			// push it out
			callback->receiver->eventHandler(&event);
			// capture any user changes to the dictionary
			stored_event.dict = event.dict;
		}
	}
}

void poEventCenter::notifyFiltered(poEvent event, const std::set<poObject*> &filter, bool exclude) {
	if(event.type >= events.size())
		return;
	
	std::vector<poEventCallback*> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		poEventCallback* callback = event_vec[i];
		
		if(!callback->event.source->visible)
			continue;
		
		if(!filter.empty()) {
			bool contains = filter.find(callback->event.source) != filter.end();
			if(exclude && contains)
				continue;
			else if(!exclude && !contains)
				continue;
		}
		
		poEvent &stored_event = callback->event;
		localizeEvent(stored_event, event);
		callback->receiver->eventHandler(&event);
		// capture any user changes to the dictionary
		stored_event.dict = event.dict;
	}
}

bool poEventCenter::routeBySource(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<poEventCallback*> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		poEventCallback* callback = event_vec[i];
		if(!callback->event.source->visible) {
			continue;
		}
        
		if(callback->event.source == obj) {
			localizeEvent(callback->event, event);
			callback->receiver->eventHandler(&event);
			sent = true;
		}
	}
	
	return sent;
}

bool poEventCenter::routeBySink(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<poEventCallback*> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		poEventCallback* callback = event_vec[i];
		if(!callback->event.source->visible) {
			continue;
		}
        
		if(callback->receiver == obj) {
			localizeEvent(callback->event, event);
			callback->receiver->eventHandler(&event);
			sent = true;
		}
	}
	
	return sent;
}

bool poEventCenter::objectHasEvent(poObject *obj, int eventType) {
	std::vector<poEventCallback*> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i]->event.source)
			return true;
	}
	return false;
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

void poEventCenter::addEventType(int eventType, bool isChecked) {
	resizeIfNecessary(eventType);
	bcheck_event[eventType] = isChecked;
}


// ======================= EVENT REWRITE CODE ======================= 


void        poEventCenter::notifyAllListeners( poEvent global_event )
{
	if(global_event.type >= events.size())
		return;
	
    // for all registed event listeners
	std::vector<poEventCallback*> &event_vec = events[global_event.type];
	for(int i=0; i<event_vec.size(); i++)
        notifyOneListener( event_vec[i], global_event );
}


void        poEventCenter::notifyOneListener( poEventCallback* callback, poEvent global_event )
{
    // get event stored in callback
    poEvent &stored_event = callback->event;
    poEvent sentEvent = stored_event;
    sentEvent.type = global_event.type;
    if(stored_event.source->visible)
    {
        // prep the event for sending
        localizeEvent(global_event, sentEvent);

        // push it out
        callback->receiver->eventHandler(&sentEvent);
        
        // capture any user changes to the dictionary
        stored_event.dict = sentEvent.dict;
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
     

void    poEventCenter::processMouseEvents( poEvent Event )
{
    // handles PO_MOUSE_DOWN_EVERYWHERE_EVENT, PO_MOUSE_DOWN_INSIDE_EVENT and PO_MOUSE_DOWN_OUTSIDE_EVENT
    // also handles first part of PO_MOUSE_DRAG_EVENT
    if ( Event.type == PO_MOUSE_DOWN_EVERYWHERE_EVENT )
    {
        // notify all objects listening for PO_MOUSE_DOWN_EVERYWHERE_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );

        // find single object to receive PO_MOUSE_DOWN_INSIDE_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_DOWN_INSIDE_EVENT;
        poEventCallback* callback = findTopObjectUnderPoint( PO_MOUSE_DOWN_INSIDE_EVENT, Event.position );
        if ( callback )
            notifyOneListener( callback, sentEvent );
        
        // notify objects listening for PO_MOUSE_DOWN_OUTSIDE_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_DOWN_OUTSIDE_EVENT;
        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_DOWN_OUTSIDE_EVENT];
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
        
        // set lastDragID for all objects listening to PO_MOUSE_DRAG_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_DRAG_EVENT;
        callback = findTopObjectUnderPoint( PO_MOUSE_DRAG_EVENT, Event.position );
        if ( callback )
            callback->event.source->eventMemory.lastDragID = 1;
    }
    
    // handles PO_MOUSE_UP_EVERYWHERE_EVENT, PO_MOUSE_UP_INSIDE_EVENT and PO_MOUSE_UP_OUTSIDE_EVENT
    // also handles last part of PO_MOUSE_DRAG_EVENT
    if ( Event.type == PO_MOUSE_UP_EVERYWHERE_EVENT )
    {
        // notify all objects listening for PO_MOUSE_UP_EVERYWHERE_EVENT
        poEvent sentEvent = Event;
        notifyAllListeners( sentEvent );
        
        // find single object to receive PO_MOUSE_UP_INSIDE_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_UP_INSIDE_EVENT;
        poEventCallback* callback = findTopObjectUnderPoint( PO_MOUSE_UP_INSIDE_EVENT, Event.position );
        if ( callback )
            notifyOneListener( callback, sentEvent );
        
        // notify objects listening for PO_MOUSE_UP_OUTSIDE_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_UP_OUTSIDE_EVENT;
        std::vector<poEventCallback*> &event_vec = events[PO_MOUSE_UP_OUTSIDE_EVENT];
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
        
        // unset lastDragID for all objects listening to PO_MOUSE_DRAG_EVENT
        // for mouse control, this should just be a single object in the set
        event_vec = events[PO_MOUSE_DRAG_EVENT];
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
        
        // find single object to receive PO_MOUSE_OVER_EVENT
        sentEvent = Event;
        sentEvent.type = PO_MOUSE_OVER_EVENT;
        poEventCallback* callback = findTopObjectUnderPoint( PO_MOUSE_OVER_EVENT, Event.position );
        if ( callback )
            notifyOneListener( callback, sentEvent );
        
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
    if ( Event.type == PO_MOUSE_DRAG_EVERYWHERE_EVENT )
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


void    poEventCenter::processTouchEvents( poEvent Event )
{
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
}


void    poEventCenter::processKeyEvents( poEvent Event )
{
    switch (Event.type) {
        case PO_KEY_DOWN_EVENT:
            notifyAllListeners( Event );
            break;
            
        case PO_KEY_UP_EVENT:
            notifyAllListeners( Event );
            break;
            
        default:
            // just eat it
            break;
    }
}

bool sortCallbacksByDrawOrderFunc(poEventCallback* a, poEventCallback* b) {
	return a->event.source->drawOrder() < b->event.source->drawOrder();
}

void    poEventCenter::sortCallbacksByDrawOrder()
{
    /*      type == PO_MOUSE_UP_INSIDE_EVENT		||
			type == PO_MOUSE_UP_OUTSIDE_EVENT		||
			type == PO_MOUSE_UP_EVERYWHERE_EVENT	||
			type == PO_MOUSE_DOWN_INSIDE_EVENT		||
			type == PO_MOUSE_DOWN_OUTSIDE_EVENT		||
			type == PO_MOUSE_DOWN_EVERYWHERE_EVENT	||
			type == PO_MOUSE_MOVE_EVENT				||
			type ==	PO_MOUSE_DRAG_EVENT				||
			type == PO_MOUSE_DRAG_EVERYWHERE_EVENT	||
			type == PO_MOUSE_ENTER_EVENT			||
			type == PO_MOUSE_OVER_EVENT				||
			type == PO_MOUSE_LEAVE_EVENT);*/
    
    // sort only event callback vectors where order matters
    std::sort( events[PO_MOUSE_UP_INSIDE_EVENT].begin(), events[PO_MOUSE_UP_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );

    std::sort( events[PO_MOUSE_DOWN_INSIDE_EVENT].begin(), events[PO_MOUSE_DOWN_INSIDE_EVENT].end(), sortCallbacksByDrawOrderFunc );

    std::sort( events[PO_MOUSE_DRAG_EVENT].begin(), events[PO_MOUSE_DRAG_EVENT].end(), sortCallbacksByDrawOrderFunc );
    
    std::sort( events[PO_MOUSE_OVER_EVENT].begin(), events[PO_MOUSE_OVER_EVENT].end(), sortCallbacksByDrawOrderFunc );

}


void    poEventCenter::clearDrawOrderForObjectsWithEvents()
{
    // for all event types
    for( int n=0; n<PO_LAST_EVENT; n++ )
    {
        // for every object with that type
        std::vector<poEventCallback*> &event_vec = events[n];
        for( int i=0; i<event_vec.size(); i++ )
        {
            poEventCallback* callback = event_vec[i];
            poObject* obj = callback->event.source;
            obj->clearDrawOrder();
        }
    }
}




