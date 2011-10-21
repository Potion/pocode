/*
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poEnums.h"
#include "poDictionary.h"

class poObject;
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

// class poObject;

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
	poEvent(int type, poObject* from, std::string _message, const poDictionary& dict=poDictionary());
	poEvent &operator=(const poEvent &e);
	
    // EVENT PROPERTIES
    // When you call addEvent(), the object that the event is added to is the source.
	// Source is the poObject that generated the event.
	poObject*       source;

	// When the event happened.
	double          timestamp;

    // The event message and dictionary are set by the user per event, via the addEvent method.
    std::string     message;
	poDictionary    dictionary;
    
	// The event type.
	int             type;

	// Mouse or key modifiers.
	unsigned int    modifiers;
	
	// position is the global (screen) position of the mouse or touch event.
    // local_position is the mouse or touch position relative to the object.
	poPoint         position, globalPosition, previousPosition;

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

};




