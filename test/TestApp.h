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
	virtual void draw();
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary &dict=poDictionary());
};

