#pragma once

#include "application/poApplication.h"
#include "poObject.h"
#include "poResource.h"

extern void setupApplication();
extern void cleanupApplication();

class TestObj 
:	public poObject
{
public:
	TestObj();
	
	virtual void update();
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	int drag_event_id;
	poResourceStore resources;
};
