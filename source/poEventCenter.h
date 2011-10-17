//
//  poEventCenter.cpp
//  potionCode
//
//  Created by Jared Schiffman on 10/16/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poEvent.h"


// CLASS NOTES
//
// The poEventCenter provides methods to the poWindow system that allow for distribution
// and propagation of all poEvents to the poObjects that should receive them.
//
// The poEventCenter is a singleton that is accesible by calling poEventCenter::get().
//
// The poEventCenter is where all poEvents are registered. When an actual events occurs, it checks
// to see if there is a recipient for that event, and then delivers it to the poObject.
//
// The poEventCenter is relatively complex in functionality and should only be altered
// when absolutely necessary.
//

struct poEventCallback {
    int uid;
    poObject *receiver;
    poEvent event;
};

struct poEventMemory
{
public:
    poEventMemory()
    {
        lastInsideTouchID = -1;
        lastDragID = -1;
    }
    
    int     lastInsideTouchID;
    int     lastDragID;
};

class poEventCenter {
public:

	static poEventCenter *get();
    
    // EVENT REGISTRATION
	// get window events to your event handler
	int     registerForEvent(int eventType, poObject *source, std::string message, const poDictionary& dict=poDictionary());
	// get window events for an object delivered to another object
	int     registerForEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict=poDictionary());
	
    // EVENT REMOVAL
	// remove a specific event by id
	void    removeEvent(int event_id);
	// get rid of everything that this obj is associated with
	void    removeAllEvents(poObject* obj);
	
    // EVENT NOTIFICATION and ROUTING
	// tell anyone who cares that this happened
	void    notify(poEvent event);
    
	// same but filters the receivers
	// true	=omit those on the filter list
	// false=omit those off the filter list
	void    notifyFiltered(poEvent event, const std::set<poObject*> &filter, bool exclude);
    
	// check if this obj wants to get this event, then send it
	// return if the event was sent
	bool    routeBySource(poObject *obj, poEvent event);
	bool    routeBySink(poObject *obj, poEvent event);
	
    // EVENTS PER OBJECT
	// does an object care about a given event
	bool    objectHasEvent(poObject *obj, int eventType);
	// get the stored event for this this object/action
	std::vector<poEvent*> eventsForObject(poObject *obj, int eventType);
	
    // NEW EVENT TYPE REGISTRATION
	// append this event type to the list ... should be an int >= PO_LAST_EVENT 
	void    addEventType(int eventType, bool isChecked);
    
    
    // EVENT REWRITE CODE
    
    void                notifyAllListeners( poEvent global_event );
    void                notifyOneListener( poEventCallback* callback, poEvent global_event );
    
    poEventCallback*    findTopObjectUnderPoint( int eventType, poPoint P );
    void                processMouseEvents( poEvent Event );
    void                processKeyEvents( poEvent Event );
    
    void                sortCallbacksByDrawOrder();
    void                clearDrawOrderForObjectsWithEvents();
	
private:
	poEventCenter();
	void resizeIfNecessary(int incoming);
	
	std::vector< bool > bcheck_event;
	std::vector< std::vector<poEventCallback*> > events;
};

static int registerEvent(int eventType, poObject *source, std::string message, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, message, dict);
}

static int registerEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, sink, message, dict);
}
