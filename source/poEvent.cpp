#include "poEvent.h"
#include "poObject.h"
#include "poApplication.h"

bool isNumericMask(unsigned int mod) {
	return (mod & PO_NUMERIC_KEY_MASK) > 0;
}

bool isFunctionMask(unsigned int mod) {
	return (mod & PO_FUNCTION_KEY_MASK) > 0;
}

bool isArrowKey(unsigned int mod) {
	return (mod & PO_NUMERIC_KEY_MASK) > 0 && (mod & PO_FUNCTION_KEY_MASK) > 0;
}

bool isMetaMask(unsigned int mod) {
	return (mod & PO_KEY_META);
}

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
			type == PO_MOUSE_DRAG_EVENT     ||
            type == PO_MOUSE_DRAG_EVERYWHERE_EVENT );
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
    tolocal.message = stored.message;
	
	if(isMouseEvent(stored.type)) {
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

template <typename T>
bool sortByDrawOrder(T *a, T *b) {
	return a->event.source->drawOrder() < b->event.source->drawOrder();
}

poObject *poEventCenter::notify(poEvent event) {
	if(event.type >= events.size())
		return NULL;
	
	if(bcheck_event[event.type]) {
		std::vector<event_callback> &event_vec = events[event.type];
		std::vector<event_callback*> possibles;
		
		for(int i=0; i<event_vec.size(); i++) {
			event_callback &callback = event_vec[i];
            
			if(!(callback.event.source->isInWindow() && callback.event.source->visible())) {
				continue;
			}
			
			// this one is an option
			if(callback.event.source->pointInside(event.position,true)) {
				possibles.push_back(&callback);
			}
		}
		
		if(!possibles.empty()) {
			// sort callback sources by drawOrder
			std::sort(possibles.begin(), possibles.end(), boost::bind(sortByDrawOrder<event_callback>, _1, _2));
			// the one closest to the top will be last
			event_callback *the_one = possibles.back();
			// put it in terms of this object
			localizeEvent(the_one->event, event);
			// and push it to the handler
			the_one->receiver->eventHandler(&event);
			// capture any user changes to the dictionary
			the_one->event.dict = event.dict;
			// tell the container we found someone
			return the_one->event.source;
		}
	}
	else {
		std::vector<event_callback> &event_vec = events[event.type];
		for(int i=0; i<event_vec.size(); i++) {
			event_callback &callback = event_vec[i];
			
			if(!(callback.event.source->isInWindow() && callback.event.source->visible())) {
				continue;
			}

			poEvent &stored_event = callback.event;
			localizeEvent(stored_event, event);
			callback.receiver->eventHandler(&event);
			// capture any user changes to the dictionary
			stored_event.dict = event.dict;
		}
	}
	
	return NULL;
}

bool poEventCenter::routeBySource(poObject *obj, poEvent event) {
	bool sent = false;
	
	std::vector<event_callback> &event_vec = events[event.type];
	for(int i=0; i<event_vec.size(); i++) {
		event_callback &callback = event_vec[i];
		if(!(callback.event.source->isInWindow() && callback.event.source->visible())) {
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
		if(!(callback.event.source->isInWindow() && callback.event.source->visible())) {
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

