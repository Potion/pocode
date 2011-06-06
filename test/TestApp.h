#pragma once

#include "poObject.h"

extern poObject* createObjectForID(uint uid);
extern void setupApplication();
extern void cleanupApplication();

class TestObj :	public poObject
{
public:
	TestObj();
	virtual void eventHandler(poEvent *event);
};

