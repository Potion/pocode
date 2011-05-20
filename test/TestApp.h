#pragma once

#include "application/poApplication.h"
#include "poObject.h"
#include "poResource.h"
#include "poCamera.h"

extern poObject* createObjectForID(uint uid);
extern void setupApplication();
extern void cleanupApplication();

class TestObj :	public poObject
{
public:
	TestObj();
	
	virtual void update();
    virtual void preDraw();
	virtual void draw();
    virtual void postDraw();
	
	virtual void eventHandler(poEvent *event);
    
	poResourceStore resources;
};

