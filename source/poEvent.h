/*
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poDictionary.h"

class poEvent;
typedef std::vector<poEvent*> poEventVec;
typedef std::vector<poEventVec> poEventTable;

enum {
	PO_EVENT=0,
	
	PO_TOUCH_BEGAN_EVENT,
	PO_TOUCH_ENDED_EVENT,
	PO_TOUCH_MOVED_EVENT,

	PO_KEY_UP_EVENT,
	PO_KEY_DOWN_EVENT,
	PO_KEY_PRESS_EVENT,
	PO_KEY_RELEASE_EVENT,

	PO_MOUSE_UP_EVENT,
	PO_MOUSE_DOWN_EVENT,
	PO_MOUSE_MOVE_EVENT,
	PO_MOUSE_PRESS_EVENT,
	PO_MOUSE_RELEASE_EVENT,
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	PO_MOUSE_DRAG_EVENT,
	
	PO_LAST_EVENT
};

bool isKeyEvent(int type);
bool isMouseEvent(int type);
bool isTouchEvent(int type);
bool isBoundsCheckedEvent(int type);

class poObject;

class poEvent
{
public:
	poEvent();
	poEvent(int type, poObject* from);
	poEvent(int type, poObject* from, poObject* to, const std::string& msg, const poDictionary& dict=poDictionary());
	
	poObject* receiver;
	poObject* sender;
	
	int type;

	unsigned int modifiers;

	// mouse events
	float x, y;
	float localX, localY;

	// key events
	int keyCode;
	char keyChar;

	// touch events
	uint32_t uid;
	double timestamp;
	float prevX, prevY;

	std::string message;
	poDictionary dict;
};
