#include "poEvent.h"

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
	return (type == PO_MOUSE_PRESS_EVENT	||
			type == PO_MOUSE_RELEASE_EVENT	||
			type == PO_MOUSE_ENTER_EVENT);
}

poEvent::poEvent() 
{
	this->type = PO_EVENT;
	this->receiver = NULL;
	this->sender = NULL;
	this->message = "";
}

poEvent::poEvent(int type, poObject* from)
{
	this->type = type;
	this->receiver = NULL;
	this->sender = from;
	this->message = "";
}

poEvent::poEvent(int type, poObject* from, poObject* to, const std::     string& msg, const poDictionary& dict)
{
	this->type = type;
	this->receiver = to;
	this->sender = from;
	this->dict = dict;
	this->message = msg;
}

