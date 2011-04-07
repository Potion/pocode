#include "poEvent.h"
#include "poObject.h"

bool isKeyEvent(int type) {
	return (type == PO_KEY_UP_EVENT			||
			type == PO_KEY_DOWN_EVENT		||
			type == PO_KEY_PRESS_EVENT		||
			type == PO_KEY_RELEASE_EVENT);
}

bool isMouseEvent(int type) {
	return (type == PO_MOUSE_UP_EVENT		||
			type == PO_MOUSE_DOWN_EVENT		||
			type == PO_MOUSE_RELEASE_EVENT	||
			type == PO_MOUSE_PRESS_EVENT	||
			type == PO_MOUSE_ENTER_EVENT	||
			type == PO_MOUSE_LEAVE_EVENT	||
			type == PO_MOUSE_MOVE_EVENT		||
			type == PO_MOUSE_DRAG_EVENT);
}

bool isTouchEvent(int type) {
	return (type == PO_TOUCH_BEGAN_EVENT	||
			type == PO_TOUCH_ENDED_EVENT	||
			type == PO_TOUCH_MOVED_EVENT);
}

bool isBoundsCheckedEvent(int type) {
	return	(type == PO_MOUSE_PRESS_EVENT);
}

void localizeEvent(poEvent &stored, poEvent &tolocal) {
	tolocal.source = stored.source;
	tolocal.dict = stored.dict;
	
	if(isMouseEvent(stored.type)) {
		poPoint pt = stored.source->globalToLocal(poPoint(tolocal.x, tolocal.y));
		tolocal.localX = pt.x;
		tolocal.localY = pt.y;
	}
}

poEvent::poEvent()
:	type(PO_LAST_EVENT)
,	source(NULL)
,	keyCode(0)
,	keyChar(0)
,	x(0.f), y(0.f)
,	localX(0.f), localY(0.f)
,	uid(0)
,	timestamp(0.0)
,	prevX(0.f), prevY(0.f)
,	modifiers(0)
{}

poEvent::poEvent(const poEvent &e)
:	type(e.type)
,	source(e.source)
,	keyCode(e.keyCode)
,	keyChar(e.keyChar)
,	x(e.x), y(e.y)
,	localX(e.localX), localY(e.localY)
,	uid(e.uid)
,	timestamp(e.timestamp)
,	prevX(e.prevX), prevY(e.prevY)
,	modifiers(e.modifiers)
,	dict(e.dict)
{}

poEvent::poEvent(int type, poObject* from, const poDictionary& dict)
:	type(type)
,	source(from)
,	keyCode(0)
,	keyChar(0)
,	x(0.f), y(0.f)
,	localX(0.f), localY(0.f)
,	uid(0)
,	timestamp(0.0)
,	prevX(0.f), prevY(0.f)
,	modifiers(0)
,	dict(dict)
{}

poEvent &poEvent::operator=(const poEvent &e) {
	type = e.type;
	source = e.source;
	keyCode = e.keyCode;
	keyChar = e.keyChar;
	x = e.x;
	y = e.y;
	localX = e.localX;
	localY = e.localY;
	uid = e.uid;
	timestamp = e.timestamp;
	prevX = e.prevX;
	prevY = e.prevY;
	modifiers = e.modifiers;
	dict = e.dict;
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
	for(int i=0; i<PO_LAST_EVENT; i++)
		bcheck_event[i] = isBoundsCheckedEvent(i);
}

int poEventCenter::registerForEvent(int eventType, poObject *source, const poDictionary& dict) {
	return registerForEvent(eventType, source, source);
}

int poEventCenter::registerForEvent(int eventType, poObject *source, poObject *sink, const poDictionary& dict) {
	if(eventType >= events.size())
		return 0;
	
	static int callback_uid = 0;
	
	event_callback callback;
	callback.uid = ++callback_uid;
	callback.receiver = sink;
	callback.event = poEvent(eventType, source, dict);
	events[eventType].push_back(callback);
	
	return callback.uid;
}

void poEventCenter::removeEvent(int uid) {
	BOOST_FOREACH(std::vector<event_callback> &event_vec, events) {
		std::vector<event_callback>::iterator iter = event_vec.begin();
		while(iter != event_vec.end()) {
			if(iter->uid == uid)
				event_vec.erase(iter++);
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

poObject *poEventCenter::notify(poEvent event) {
	if(event.type >= events.size())
		return NULL;
	
	if(bcheck_event[event.type]) {
		// we go thru and store all the ones that could send it
		// then figure out which is closer to the top and send to that one
		event_callback *the_one = NULL;
		
		std::vector<event_callback> &event_vec = events[event.type];
		for(int i=0; i<event_vec.size(); i++) {
			event_callback &callback = event_vec[i];
			
			// this one is an option
			if(callback.event.source->pointInside(event.x, event.y, 0.f, true)) {
				// higher draw_order means closer to surface, in 2D
				if(!the_one || the_one->event.source->draw_order < callback.event.source->draw_order)
					the_one = &callback;
			}
		}
		
		if(the_one) {
			localizeEvent(the_one->event, event);
			the_one->receiver->eventHandler(&event);
			// tell the container we found someone
			return the_one->event.source;
		}
	}
	else {
		std::vector<event_callback> &event_vec = events[event.type];
		for(int i=0; i<event_vec.size(); i++) {
			event_callback &callback = event_vec[i];
			poEvent &stored_event = callback.event;

			localizeEvent(stored_event, event);
			callback.receiver->eventHandler(&event);
		}
	}
	
	return NULL;
}

bool poEventCenter::routeBySource(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
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

poEvent &poEventCenter::eventForObject(poObject *obj, int eventType) {
	std::vector<event_callback> &event_vec = events[eventType];
	for(int i=0; i<event_vec.size(); i++) {
		if(obj == event_vec[i].event.source)
			return event_vec[i].event;
	}
	
	static poEvent event;
	return event;
}

void poEventCenter::addEventType(int eventType, bool isChecked) {
	resizeIfNecessary(eventType);
	bcheck_event[eventType] = isChecked;
}

