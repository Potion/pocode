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

    // PO_EVENT SPECIAL KEYS
    enum {
        // key event mask
        PO_KEY_CAPS		= 1 << 16,
        PO_KEY_SHIFT	= 1 << 17,
        PO_KEY_CTRL		= 1 << 18,
        PO_KEY_ALT		= 1 << 19,
        PO_KEY_META		= 1 << 20,
        // specific key 
        PO_NUMERIC_KEY_MASK = 1 << 21,
        PO_FUNCTION_KEY_MASK = 1 << 23,
    };


    // PO_EVENT MOUSE BUTTON LEFT and RIGHT
    enum {
        // mouse event mask
        PO_MOUSE_LEFT	= 1,
        PO_MOUSE_MIDDLE	= 1 << 1,
        PO_MOUSE_RIGHT	= 1 << 2,
    };

    enum {
        // also need to check the modifier flags
        // for arrows, numeric mask and function mask should both be set
        // these numbers are the keyCode
        PO_UP_ARROW = 126,
        PO_DOWN_ARROW = 125,
        PO_LEFT_ARROW = 123,
        PO_RIGHT_ARROW = 124,
        
        PO_RETURN_KEY = 36,
        
        PO_F1 = 122,
        PO_F2 = 120,
        PO_F3 = 99,
        PO_F4 = 118,
        PO_F5 = 96,
        PO_F6 = 97,
        PO_F7 = 98,
        PO_F8 = 100
    };

    // class Object;

    // CLASS NOTES
    //
    // Event is an object used in pocode for communicating user events to Objects.
    //
    // Objects register for events by calling the Object's addEvent() method as follows:
    //
    //      myObject->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "message" );
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
        Event(int type, Object* from, std::string _message, const poDictionary& dict=poDictionary());
        
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
        poDictionary    dictionary;
        
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