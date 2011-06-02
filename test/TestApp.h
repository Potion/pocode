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
	
	poObject *holder;
	
	void setupAperture();
	
	void start();
	void stop();
	void open();
	void close();
};

