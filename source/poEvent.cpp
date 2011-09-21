#include "poEvent.h"
#include "poObject.h"
#include "poApplication.h"


bool isNumericMask(poEvent* e)	{return (e->modifiers & PO_NUMERIC_KEY_MASK) > 0;}
bool isFunctionMask(poEvent* e)	{return (e->modifiers & PO_FUNCTION_KEY_MASK) > 0;}
bool isArrowKey(poEvent* e)		{return isNumericMask(e) && isFunctionMask(e);}
bool isMetaMask(poEvent* e)		{return (e->modifiers & PO_KEY_META) > 0;}
bool isAltMask(poEvent* e)		{return (e->modifiers & PO_KEY_ALT) > 0;}
bool isShiftMask(poEvent* e)	{return (e->modifiers & PO_KEY_SHIFT) > 0;}
bool isCtrlMask(poEvent* e)		{return (e->modifiers & PO_KEY_CTRL) > 0;}
bool isLeftMouse(poEvent* e)	{return (e->modifiers & PO_MOUSE_LEFT) > 0;}
bool isRightMouse(poEvent* e)	{return (e->modifiers & PO_MOUSE_RIGHT) > 0;}
bool isCenterMouse(poEvent* e)	{return (e->modifiers & PO_MOUSE_MIDDLE) > 0;}

bool isKeyEvent(int type) {
	return (type == PO_KEY_UP_EVENT			||
			type == PO_KEY_DOWN_EVENT		||
			type == PO_KEY_PRESS_EVENT		||
			type == PO_KEY_RELEASE_EVENT);
}

bool isMouseEvent(int type) {
	return (type == PO_MOUSE_UP_INSIDE_EVENT		||
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
			type == PO_MOUSE_LEAVE_EVENT);
}

bool isTouchEvent(int type) {
	return (type == PO_TOUCH_BEGAN_EVERYWHERE_EVENT	||
            type == PO_TOUCH_BEGAN_INSIDE_EVENT     ||
            type == PO_TOUCH_BEGAN_OUTSIDE_EVENT	||
            type == PO_TOUCH_MOVED_EVERYWHERE_EVENT ||
            type == PO_TOUCH_MOVED_INSIDE_EVENT     ||
            type == PO_TOUCH_MOVED_OUTSIDE_EVENT    ||
			type == PO_TOUCH_ENDED_EVERYWHERE_EVENT	||
            type == PO_TOUCH_ENDED_INSIDE_EVENT     ||
            type == PO_TOUCH_ENDED_OUTSIDE_EVENT
            );
}

void localizeEvent(poEvent &stored, poEvent &tolocal) {
	tolocal.source = stored.source;
	tolocal.dict = stored.dict;
    tolocal.message = stored.message;
	
	if(isMouseEvent(stored.type) || isTouchEvent(stored.type)) {
		// flip the coords so the local position can match the orientation of the global one
		poPoint pt = stored.source->globalToLocal(poPoint(tolocal.position.x, getWindowHeight()-tolocal.position.y));
		tolocal.local_position = pt;
	}
}

poEvent::poEvent()
:	type(PO_LAST_EVENT)
,	source(NULL)
,	keyCode(0)
,	keyChar(0)
,	uid(0)
,	timestamp(0.0)
,	modifiers(0)
,   touchID(-1)
{}

poEvent::poEvent(const poEvent &e)
:	type(e.type)
,	source(e.source)
,	keyCode(e.keyCode)
,	keyChar(e.keyChar)
,	position(e.position)
,	local_position(e.local_position)
,	uid(e.uid)
,	timestamp(e.timestamp)
,	previous_position(e.previous_position)
,	modifiers(e.modifiers)
,   message(e.message)
,	dict(e.dict)
,   touchID(-1)
{}

poEvent::poEvent(int type, poObject* from, std::string _message, const poDictionary& dict)
:	type(type)
,	source(from)
,	keyCode(0)
,	keyChar(0)
,	uid(0)
,	timestamp(0.0)
,	modifiers(0)
,   message(_message)
,	dict(dict)
,   touchID(-1)
{
}

poEvent &poEvent::operator=(const poEvent &e) {
	type = e.type;
	source = e.source;
	keyCode = e.keyCode;
	keyChar = e.keyChar;
	position = e.position;
	local_position = e.local_position;
	uid = e.uid;
	timestamp = e.timestamp;
	previous_position = e.previous_position;
	modifiers = e.modifiers;
	dict = e.dict;
    message = e.message;
	touchID = e.touchID;
	return *this;
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
	
	event_callback callback;
	callback.uid = ++callback_uid;
	callback.receiver = sink;
	callback.event = poEvent(eventType, source, message, dict);
	events[eventType].push_back(callback);
    
	return callback.uid;
}

void poEventCenter::removeEvent(int uid) {
	BOOST_FOREACH(std::vector<event_callback> &event_vec, events) {
		std::vector<event_callback>::iterator iter = event_vec.begin();
		while(iter != event_vec.end()) {
			if(iter->uid == uid)
				iter = event_vec.erase(iter);
			else
				iter++;
		}
	}
}

void poEventCenter::removeAllEvents(poObject* obj) {
	BOOST_FOREACH(std::vector<event_callback> &event_vec, events) {
		std::vector<event_callback>::iterator iter = event_vec.begin();
		while(iter != event_vec.end()) {
			if(iter->event.source == obj || iter->receiver == obj)
				event_vec.erase(iter++);
			else
				iter++;
		}
	}
}

void poEventCenter::notify(poEvent event) {
	if(event.type >= events.size())
		return;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
		poEvent &stored_event = callback.event;
		if(stored_event.source->visible) {
			// prep the event for sending
			localizeEvent(stored_event, event);
			// push it out
			callback.receiver->eventHandler(&event);
			// capture any user changes to the dictionary
			stored_event.dict = event.dict;
		}
	}
}

void poEventCenter::notifyFiltered(poEvent event, const std::set<poObject*> &filter, bool exclude) {
	if(event.type >= events.size())
		return;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
		
		if(!(callback.event.source->isInWindow() && callback.event.source->visible))
			continue;
		
		if(!filter.empty()) {
			bool contains = filter.find(callback.event.source) != filter.end();
			if(exclude && contains)
				continue;
			else if(!exclude && !contains)
				continue;
		}
		
		poEvent &stored_event = callback.event;
		localizeEvent(stored_event, event);
		callback.receiver->eventHandler(&event);
		// capture any user changes to the dictionary
		stored_event.dict = event.dict;
	}
}

bool poEventCenter::routeBySource(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
		if(!(callback.event.source->isInWindow() && callback.event.source->visible)) {
			continue;
		}

		if(callback.event.source == obj) {
			localizeEvent(callback.event, event);
			callback.receiver->eventHandler(&event);
			sent = true;
		}
	}
	
	return sent;
}

bool poEventCenter::routeBySink(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
		if(!(callback.event.source->isInWindow() && callback.event.source->visible)) {
			continue;
		}

		if(callback.receiver == obj) {
			localizeEvent(callback.event, event);
			callback.receiver->eventHandler(&event);
			sent = true;
		}
	}
	
	return sent;
}

bool poEventCenter::objectHasEvent(poObject *obj, int eventType) {
	std::vector<event_callback> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i].event.source)
			return true;
	}
	return false;
}

std::vector<poEvent*> poEventCenter::eventsForObject(poObject *obj, int eventType) {
	std::vector<poEvent*> response;
	
	std::vector<event_callback> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i].event.source)
			response.push_back(&(event_vec[i].event));
	}
	return response;
}

void poEventCenter::addEventType(int eventType, bool isChecked) {
	resizeIfNecessary(eventType);
	bcheck_event[eventType] = isChecked;
}

