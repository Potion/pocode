#pragma once

#include "application/poApplication.h"
#include "poObject.h"

extern void setupApplication();
extern void cleanupApplication();

class TestObj 
:	public poObject
{
public:
	virtual void setup();
	virtual void update();
	virtual void draw();
	
	virtual bool eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary &dict=poDictionary());

	poPoint mouse;
};
