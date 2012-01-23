//
//  poEventCenter.cpp
//  pocode
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

// used internally to store relevant info between events
// e.g. dragging, etc
struct poEventMemory {
public:
    poEventMemory()
	:	lastInsideTouchID(-1)
	,	lastDragID(-1)
	{}
    
    // for enter / leave
    int     lastInsideTouchID;
	// for dragging
    int     lastDragID;
};

class poEventCenter {
public:

	static poEventCenter *get();
	
    // EVENT REGISTRATION
	// get window events to your event handler
	void     addEvent(int eventType, poObject *source, std::string message, const poDictionary& dict=poDictionary());
	// get window events for an object delivered to another object
	void     addEvent(int eventType, poObject *source, poObject *sink, std::string message, const poDictionary& dict=poDictionary());
	// get rid of everything that this obj is associated with
	void    removeAllEvents(poObject* obj);
	// will remove everything, either bouncing or generating
	void	removeAllEventsOfType(poObject* obj, int eventType);
	// does an object care about a given event
	bool    objectHasEvent(poObject *obj, int eventType);

	// handle events and remove them from the list
	void	processEvents(std::deque<poEvent> &events);
	
	// will register all events to an object that are already registered 'from' object
	void	copyEventsFromObject(poObject *from, poObject *to);
	// get the stored event for this this object/action
	std::vector<poEvent*> eventsForObject(poObject *obj, int eventType);

    // hack to make visible children with invisible parents get left out of event processing
    void negateDrawOrderForObjectWithEvents();
    
private:
	poEventCenter();
    
	// figure out which poObject is closest to the user under a point
    poEventCallback* findTopObjectUnderPoint( int eventType, poPoint P );
	void	sortCallbacksByDrawOrder();
    void    notifyAllListeners( poEvent &global_event );
	void	notifyOneListener( poEventCallback *callback, poEvent &global_event);

    void    processMouseEvents( poEvent &Event );
    void	processTouchEvents( poEvent &Event );
    void	processKeyEvents( poEvent &Event );

	bool	objectIsAvailableForEvents(poObject *obj);
	
	std::vector< std::vector<poEventCallback*> > events;
};

