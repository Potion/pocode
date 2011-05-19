#pragma once

#include "application/poApplication.h"
#include "poObject.h"
#include "poResource.h"

extern poObject* createObjectForID(uint uid);
extern void setupApplication();
extern void cleanupApplication();

class TestObj 
:	public poObject
{
public:
	TestObj();
	
	virtual void preDraw();
	virtual void eventHandler(poEvent *event);

	poResourceStore resources;
};
