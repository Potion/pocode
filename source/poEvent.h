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

class poEvent
{
public:
	poEvent();
	poEvent(const poEvent &e);
	poEvent(int type, poObject* from, std::string _message, const poDictionary& dict=poDictionary());
	poEvent &operator=(const poEvent &e);
	
	// the object that generated the event
	poObject* source;
	
	// event type
	int type;

	// mouse or key modifiers
	unsigned int modifiers;
	
	// mouse events
	poPoint position, local_position;

	// key events
	int keyCode;
	// with modifiers applied
	int keyChar;
	
	// multitouch ID
	int touchID;

	// touch events
	uint32_t uid;
	double timestamp;
	poPoint previous_position;

	// other info, at your discretion
    std::string message;
	poDictionary dict;
};

class poEventCenter {
public:
	static poEventCenter *get();

	// get window events to your event handler
	int registerForEvent(int eventType, poObject *source, std::string message, const poDictionary& dict=poDictionary());
	// get window events for an object delivered to another object
	int registerForEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict=poDictionary());
	
	// remove a specific event by id
	void removeEvent(int event_id);
	// get rid of everything that this obj is associated with
	void removeAllEvents(poObject* obj);
	
	// tell anyone who cares that this happened
	void notify(poEvent event);
	// same but filters the receivers
	// true	=omit those on the filter list
	// false=omit those off the filter list
	void notifyFiltered(poEvent event, const std::set<poObject*> &filter, bool exclude);
	// check if this obj wants to get this event, then send it
	// return if the event was sent
	bool routeBySource(poObject *obj, poEvent event);
	bool routeBySink(poObject *obj, poEvent event);
	
	// does an object care about a given event
	bool objectHasEvent(poObject *obj, int eventType);
	// get the stored event for this this object/action
	poEvent &eventForObject(poObject *obj, int eventType);
	
	// append this event type to the list ... should be an int >= PO_LAST_EVENT 
	void addEventType(int eventType, bool isChecked);
	
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
