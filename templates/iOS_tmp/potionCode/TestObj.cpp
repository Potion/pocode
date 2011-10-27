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

<<<<<<< HEAD
	
	poObject *obj = addChild(new poObject());
	obj->visible = false;
	
	poShape2D *shp = obj->addChild(new poRectShape(50,50));
=======
	poShape2D *shp = addChild(new poRectShape(50,50));
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
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
<<<<<<< HEAD
	
	shp = addChild(new poRectShape(500,500));
	shp->addEvent(PO_TOUCH_BEGAN_INSIDE_EVENT, this, "", poDictionary().set("id",2));
	shp->fillColor = poColor(.5,.5,.5,.5);
=======
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_TOUCH_BEGAN_INSIDE_EVENT) {
<<<<<<< HEAD
		printf("shape %d, touch %d, began inside ** %d\n", event->dictionary.getInt("id"), event->touchID, event->source->drawOrder());
		if(event->dictionary.getInt("id") != 2)	
			((poShape2D*)event->source)->fillColor = poColor::green;
=======
		printf("shape %d, touch %d, began inside\n", event->dictionary.getInt("id"), event->touchID);
		((poShape2D*)event->source)->fillColor = poColor::green;
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
	}
	
	else 
	if(event->type == PO_TOUCH_BEGAN_OUTSIDE_EVENT) {
<<<<<<< HEAD
		printf("shape %d, touch %d, began outside ** %d\n", event->dictionary.getInt("id"), event->touchID, event->source->drawOrder());
=======
		printf("shape %d, touch %d, began outside\n", event->dictionary.getInt("id"), event->touchID);
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
		((poShape2D*)event->source)->fillColor = poColor::red;
	}
	
	else 
	if(event->type == PO_TOUCH_ENDED_INSIDE_EVENT) {
<<<<<<< HEAD
		printf("shape %d, touch %d, ended inside ** %d\n", event->dictionary.getInt("id"), event->touchID, event->source->drawOrder());
=======
		printf("shape %d, touch %d, ended inside\n", event->dictionary.getInt("id"), event->touchID);
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
		((poShape2D*)event->source)->fillColor = poColor::white;
	}

	else 
	if(event->type == PO_TOUCH_ENDED_OUTSIDE_EVENT) {
<<<<<<< HEAD
		printf("shape %d, touch %d, ended outside ** %d\n", event->dictionary.getInt("id"), event->touchID, event->source->drawOrder());
=======
		printf("shape %d, touch %d, ended outside\n", event->dictionary.getInt("id"), event->touchID);
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
		((poShape2D*)event->source)->fillColor = poColor::orange;
	}
}




