//
//  TestObj.cpp
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "TestObj.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poApplication.h"
#include "Helpers.h"
#include "poMask.h"

poObject *createObjectForID(uint id) {
	return new TestObj();
}

TestObj::TestObj() {
	addModifier(new poCamera2D(poColor::green));
    
    shape = addChild(new poRectShape("apple.jpg"));
    shape->fillColor.set255(255,0,0);
	shape->alignment(PO_ALIGN_CENTER_CENTER);
    shape->addEvent(PO_TOUCH_BEGAN_INSIDE_EVENT, this);
	
	poShape2D *mask = new poOvalShape(100, 100, 100);
	mask->offset = shape->getBounds().getSize()/2.f;
	shape->addModifier(new poGeometryMask(mask));
}

void TestObj::update() {
	static float phase = 0.f;
	
	poPoint size = getWindowDimensions();
	shape->position.x = size.x / 2.f + cosf(phase) * (size.x / 3.f);
	shape->position.y = size.y / 2.f + sinf(phase) * (size.y / 3.f);
	
	phase += 0.01f;
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_TOUCH_BEGAN_INSIDE_EVENT:
			std::cout << event->touchID << std::endl;
			break;
	}
}


