#include "poEvent.h"

int poEvent::NUM_EVENTS = 11;

poEvent::poEvent() 
{
	this->type = PO_EVENT;
	this->receiver = NULL;
	this->sender = NULL;
	this->message = NULL;
	
	mouseWasInside = false;
	checkBounds = false;
}

poEvent::poEvent(int type, poObject* from)
{
	this->type = type;
	this->receiver = NULL;
	this->sender = from;
	message = new char[1];
	memset(message, 0, 1);
	
	mouseWasInside = false;
	checkBounds = false;
}

poEvent::poEvent(int type, poObject* from, poObject* to, const char* msg, const poDictionary& dict)
{
	this->type = type;
	this->receiver = to;
	this->sender = from;
	this->dict = dict;
	message = new char[strlen(msg)+1];
	strcpy(message, msg);

	mouseWasInside = false;
	checkBounds = false;
}

poEvent::poEvent(const poEvent& rhs)
{
	message = new char[strlen(rhs.message)+1];
	strcpy(message, rhs.message);
}

poEvent& poEvent::operator=(const poEvent& rhs)
{
	message = new char[strlen(rhs.message)+1];
	strcpy(message, rhs.message);
	return *this;
}

poEvent::~poEvent()
{
	if(message != NULL)
		delete [] message;
}
