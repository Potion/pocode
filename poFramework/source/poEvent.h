/*
 *  poEvent.h
 *  poFramework2
 *
 *  Created by Joshua Fisher on 9/20/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poCommon.h"
#include "poDictionary.h"

#ifndef _PO_EVENT_H_
#define _PO_EVENT_H_

enum {
	PO_EVENT=0,
	PO_TICK_EVENT,
	PO_KEY_DOWN_EVENT,
	PO_MOUSE_MOVE_EVENT,
	PO_MOUSE_DOWN_EVENT,
	PO_MOUSE_UP_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	PO_MOUSE_DRAG_EVENT,
	PO_MOUSE_OVER_EVENT,
	// EVENTS BELOW THIS POINT REQUIRE BOUNDS CHECKING BY DEFAULT
	PO_MOUSE_PRESS_EVENT,
	PO_MOUSE_RELEASE_EVENT,
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_RELEASE_OUTSIDE_EVENT,
};


class poObject;

class poEvent
{
public:
	static int NUM_EVENTS;
	static int registerNewEvent() { return ++NUM_EVENTS; }



	poEvent();
	poEvent(int type, poObject* from);
	poEvent(int type, poObject* from, poObject* to, const char* msg, const poDictionary& dict=poDictionary());
	poEvent(const poEvent& rhs);
	poEvent& operator=(const poEvent& rhs);
	virtual ~poEvent();
	
	poObject* receiver;
	poObject* sender;
	int type;
	float x, y;
	float localX, localY;
	int key;
	unsigned int modifiers;

	char* message;
	poDictionary dict;
	
	bool mouseWasInside;
	bool checkBounds;
};


#include <vector>
typedef std::vector<poEvent*> poEventVector;
typedef std::vector<poEventVector> poEventTable;

#endif