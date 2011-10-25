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
#include "poResourceLoader.h"
#include "poGeometryMask.h"

poObject *createObjectForID(uint id) {
	return new TestObj();
}

TestObj::TestObj() {
	addModifier(new poCamera2D());

	poShape2D *shp = addChild(new poRectShape(50,50));
	shp->position.set(200,100,0);
	shp->addEvent(PO_TOUCH_BEGAN_INSIDE_EVENT, this, "", poDictionary().set("id",0));
	shp->addEvent(PO_TOUCH_BEGAN_OUTSIDE_EVENT, this, "", poDictionary().set("id",0));
	shp->addEvent(PO_TOUCH_ENDED_INSIDE_EVENT, this, "", poDictionary().set("id",0));
	shp->addEvent(PO_TOUCH_ENDED_OUTSIDE_EVENT, this, "", poDictionary().set("id",0));
	
	shp = addChild(new poRectShape(50,50));
	shp->position.set(600,100,0);
	shp->addEvent(PO_TOUCH_BEGAN_INSIDE_EVENT, this, "", poDictionary().set("id",1));
	shp->addEvent(PO_TOUCH_BEGAN_OUTSIDE_EVENT, this, "", poDictionary().set("id",1));
	shp->addEvent(PO_TOUCH_ENDED_INSIDE_EVENT, this, "", poDictionary().set("id",1));
	shp->addEvent(PO_TOUCH_ENDED_OUTSIDE_EVENT, this, "", poDictionary().set("id",1));
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_TOUCH_BEGAN_INSIDE_EVENT) {
		printf("shape %d, touch %d, began inside\n", event->dictionary.getInt("id"), event->touchID);
		((poShape2D*)event->source)->fillColor = poColor::green;
	}
	
	else 
	if(event->type == PO_TOUCH_BEGAN_OUTSIDE_EVENT) {
		printf("shape %d, touch %d, began outside\n", event->dictionary.getInt("id"), event->touchID);
		((poShape2D*)event->source)->fillColor = poColor::red;
	}
	
	else 
	if(event->type == PO_TOUCH_ENDED_INSIDE_EVENT) {
		printf("shape %d, touch %d, ended inside\n", event->dictionary.getInt("id"), event->touchID);
		((poShape2D*)event->source)->fillColor = poColor::white;
	}

	else 
	if(event->type == PO_TOUCH_ENDED_OUTSIDE_EVENT) {
		printf("shape %d, touch %d, ended outside\n", event->dictionary.getInt("id"), event->touchID);
		((poShape2D*)event->source)->fillColor = poColor::orange;
	}
}




