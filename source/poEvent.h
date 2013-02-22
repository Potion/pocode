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

#pragma once


#include "poEnums.h"
#include "poDictionary.h"

namespace po {
    class Object;
    class Event;

    typedef std::vector<Event*> EventVec;
    typedef std::vector<EventVec> EventTable;

    // check to see if a specific modifier key is being held down
    bool isNumericMask(Event*);
    bool isFunctionMask(Event*);
    bool isArrowMask(Event*);
    bool isMetaMask(Event*);
    bool isAltMask(Event*);
    bool isShiftMask(Event*);
    bool isCtrlMask(Event*);

    // check for a given mouse button
    bool isLeftMouseDown(Event*);
    bool isRightMouseDown(Event*);
    bool isCenterMouseDown(Event*);

    // check event type
    bool isMouseEvent(int type);
    bool isKeyEvent(int type);
    bool isTouchEvent(int type);
    bool isMotionEvent(int type);

    // class Object;

    // CLASS NOTES
    //
    // Event is an object used in pocode for communicating user events to Objects.
    //
    // Objects register for events by calling the Object's addEvent() method as follows:
    //
    //      myObject->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this, "message" );
    //
    // Objects receive events through the virtual eventHandler() method.
    //
    // Event objects are used as records of event registration (stored by the EventCenter)
    // and as containers for event messages that are propagated to Objects.
    //
    // In general, users do not need to construct their own Events.
    //


    class Event
    {
    public:
        Event();
        Event(int type, Object* from, std::string _message, const Dictionary& dict=Dictionary());
        
        // EVENT PROPERTIES
        // When you call addEvent(), the object that the event is added to is the source.
        // Source is the Object that generated the event.
        Object*       source;

        // The event type.
        int             type;
        // When the event happened.
        float          timestamp;

        // The event message and dictionary are set by the user per event, via the addEvent method.
        std::string     message;
        Dictionary    dictionary;
        
        // Mouse or key modifiers.
        unsigned int    modifiers;
        
        // position is local to the receiver of the event.
        // globalPosition is the mouse or touch position relative to the screen.
        Point       localPosition, globalPosition, previousPosition;
        
        //Accelerometer, gyro, etc
        Point         motion;

        // The keyCode is the raw key identifier, OS-defined.
        int             keyCode;
        // The keyChar is the key with modifiers applied.
        // In general, the keyChar is used over keyCode
        int             keyChar;
        
        // The touchID stores the identifier of a touch event. All touch events connected
        // to a single touch movement should have the same touchID.
        int             touchID;
        int             uniqueID;
        // Touches have a tap count in iOS, useful for double/triple tap events
        int             tapCount;
    };
} /*End po namespace */