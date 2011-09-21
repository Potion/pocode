/*
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poEnums.h"
#include "poDictionary.h"

class poEvent;
typedef std::vector<poEvent*> poEventVec;
typedef std::vector<poEventVec> poEventTable;

bool isNumericMask(poEvent*);
bool isFunctionMask(poEvent*);
bool isArrowKey(poEvent*);
bool isMetaMask(poEvent*);
bool isAltMask(poEvent*);
bool isShiftMask(poEvent*);
bool isCtrlMask(poEvent*);
bool isLeftMouse(poEvent*);
bool isRightMouse(poEvent*);
bool isCenterMouse(poEvent*);

bool isKeyEvent(int type);
bool isMouseEvent(int type);
bool isTouchEvent(int type);

class poObject;

// CLASS NOTES
//
// poEvent is an object used in potionCode for communicating user events to poObjects.
//
// poObjects register for events by calling the poObject's addEvent() method as follows:
//
//      myObject->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "message" );
//
// poObjects receive events through the virtual eventHandler() method.
//
// poEvent objects are used as records of event registration (stored by the poEventCenter)
// and as containers for event messages that are propagated to poObjects.
//
// In general, users do not need to construct their own poEvents.
//


class poEvent
{
public:
	poEvent();
	poEvent(const poEvent &e);
	poEvent(int type, poObject* from, std::string _message, const poDictionary& dict=poDictionary());
	poEvent &operator=(const poEvent &e);
	
    // EVENT PROPERTIES
    // When you call addEvent(), the object that the event is added to is the source.
	// Source is the poObject that generated the event.
	poObject*       source;
	
    // The event message and dictionary are set by the user per event, via the addEvent method.
    std::string     message;
	poDictionary    dict;
    
	// The event type.
	int             type;

	// Mouse or key modifiers.
	unsigned int    modifiers;
	
	// position is the global (screen) position of the mouse or touch event.
    // local_position is the mouse or touch position relative to the object.
	poPoint         position, local_position;

	// The keyCode is the raw key identifier.
	int             keyCode;
    
	// The keyChar is the key with modifiers applied.
    // In general, the keyChar is used over keyCode/
	int             keyChar;
	
	// The touchID stores the identifier of a touch event. All touch events connected
    // to a single touch movement should have the same touchID.
	int             touchID;
    
    //Touches have a tap count in iOS, useful for double/triple tap events
    int             tapCount;

	// These properties are used for event tracking.
	uint32_t        uid;
	double          timestamp;
	poPoint         previous_position;
};




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
	
private:
	poEventCenter();
	void resizeIfNecessary(int incoming);
	
	struct event_callback {
		int uid;
		poObject *receiver;
		poEvent event;
	};
	
	std::vector< bool > bcheck_event;
	std::vector< std::vector<event_callback> > events;
};

static int registerEvent(int eventType, poObject *source, std::string message, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, message, dict);
}

static int registerEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, sink, message, dict);
}


