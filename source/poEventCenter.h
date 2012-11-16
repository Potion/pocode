/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  EventCenter.cpp
//  pocode
//
//  Created by Jared Schiffman on 10/16/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poEvent.h"


// CLASS NOTES
//
// The EventCenter provides methods to the poWindow system that allow for distribution
// and propagation of all Events to the Objects that should receive them.
//
// The EventCenter is a singleton that is accesible by calling EventCenter::get().
//
// The EventCenter is where all Events are registered. When an actual events occurs, it checks
// to see if there is a recipient for that event, and then delivers it to the Object.
//
// The EventCenter is relatively complex in functionality and should only be altered
// when absolutely necessary.
//

namespace po {
    struct EventCallback {
        int uid;
        Object *receiver;
        Event event;
    };

    // used internally to store relevant info between events
    // e.g. dragging, etc
    struct EventMemory {
    public:
        EventMemory()
        :	lastInsideTouchID(-1)
        ,	lastDragID(-1)
        {}
        
        // for enter / leave
        int     lastInsideTouchID;
        // for dragging
        int     lastDragID;
    };

    class EventCenter {
    public:

        static EventCenter *get();
        
        // EVENT REGISTRATION
        // get window events to your event handler
        void     addEvent(int eventType, Object *source, std::string message, const poDictionary& dict=poDictionary());
        // get window events for an object delivered to another object
        void     addEvent(int eventType, Object *source, Object *sink, std::string message, const poDictionary& dict=poDictionary());
        // get rid of everything that this obj is associated with
        void    removeAllEvents(Object* obj);
        // will remove everything, either bouncing or generating
        void	removeAllEventsOfType(Object* obj, int eventType);
        // does an object care about a given event
        bool    objectHasEvent(Object *obj, int eventType);

        // handle events and remove them from the list
        void	processEvents(std::deque<Event> &events);
        
        // will register all events to an object that are already registered 'from' object
        void	copyEventsFromObject(Object *from, Object *to);
        // get the stored event for this this object/action
        std::vector<Event*> eventsForObject(Object *obj, int eventType);

        // hack to make visible children with invisible parents get left out of event processing
        void negateDrawOrderForObjectWithEvents();
        
    private:
        EventCenter();
        
        // figure out which Object is closest to the user under a point
        EventCallback* findTopObjectUnderPoint( int eventType, Point P );
        void	sortCallbacksByDrawOrder();
        void    notifyAllListeners( Event &global_event );
        void	notifyOneListener( EventCallback *callback, Event &global_event);

        void    processMouseEvents( Event &Event );
        void	processTouchEvents( Event &Event );
        void	processKeyEvents( Event &Event );
        void	processMotionEvent( Event &Event );

        bool	objectIsAvailableForEvents(Object *obj);
        
        std::vector< std::vector<EventCallback*> > events;
    };

} /*End po namespace */