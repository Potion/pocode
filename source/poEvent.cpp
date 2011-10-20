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

