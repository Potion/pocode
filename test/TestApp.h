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
	
	virtual void eventHandler(poEvent *event);

	poPoint mouse;
};
