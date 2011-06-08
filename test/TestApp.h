#pragma once

#include "poObject.h"

extern poObject* createObjectForID(uint uid);
extern void setupApplication();
extern void cleanupApplication();

class poTextBox;

class TestObj :	public poObject
{
public:
	TestObj();
	virtual void update();
	virtual void eventHandler(poEvent *event);
	
private:
	poTextBox *tb;
};

