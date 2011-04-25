/*
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poDictionary.h"

class poEvent;
typedef std::vector<poEvent*> poEventVec;
typedef std::vector<poEventVec> poEventTable;

enum {
	PO_TOUCH_BEGAN_EVENT = 0,
	PO_TOUCH_ENDED_EVENT,
	PO_TOUCH_MOVED_EVENT,

	PO_KEY_UP_EVENT,
	PO_KEY_DOWN_EVENT,
	PO_KEY_PRESS_EVENT,
	PO_KEY_RELEASE_EVENT,

	PO_MOUSE_UP_EVENT,
	PO_MOUSE_DOWN_EVENT,
	PO_MOUSE_MOVE_EVENT,
	PO_MOUSE_DRAG_EVENT,
	PO_MOUSE_PRESS_EVENT,
	PO_MOUSE_RELEASE_EVENT,
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	
	PO_LAST_EVENT
};

enum {
	// key event mask
	PO_KEY_CTRL		= 1,
	PO_KEY_SHIFT	= 2,
	PO_KEY_ALT		= 2 << 1,
	PO_KEY_META		= 2 << 2,
	// mouse event mask
	PO_MOUSE_LEFT	= 2 << 3,
	PO_MOUSE_MIDDLE	= 2 << 4,
	PO_MOUSE_RIGHT	= 2 << 5
};

bool isKeyEvent(int type);
bool isMouseEvent(int type);
bool isTouchEvent(int type);

class poObject;

class poEvent
{
public:
	poEvent();
	poEvent(const poEvent &e);
	poEvent(int type, poObject* from, const poDictionary& dict=poDictionary());
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
	char keyChar;

	// touch events
	uint32_t uid;
	double timestamp;
	poPoint previous_position;

	// other info, at your discretion
	poDictionary dict;
};

class poEventCenter {
public:
	static poEventCenter *get();

	// get window events to your event handler
	int registerForEvent(int eventType, poObject *source, const poDictionary& dict=poDictionary());
	// get window events for an object delivered to another object
	int registerForEvent(int eventType, poObject *source, poObject *sink, const poDictionary& dict=poDictionary());
	
	// remove a specific event by id
	void removeEvent(int event_id);
	// get rid of everything that this obj is associated with
	void removeAllEvents(poObject* obj);
	
	// tell anyone who cares that this happened
	// returns the object that will generate the event, if its bounds checked
	poObject *notify(poEvent event);
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

static int registerEvent(int eventType, poObject *source, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, dict);
}

static int registerEvent(int eventType, poObject *source, poObject *sink, const poDictionary& dict=poDictionary()) {
	return poEventCenter::get()->registerForEvent(eventType, source, sink, dict);
}
