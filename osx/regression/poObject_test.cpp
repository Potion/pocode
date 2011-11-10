//
//  poObject_test.cpp
//  potionCode
//
//  Created by filippo on 11/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poObject.h"
#include "poShapeBasics2D.h"

BOOST_AUTO_TEST_CASE( poObject_children ) {
	// CHILDREN
	poObject* parent = new poObject();
	poObject* child = new poObject();
	child->name = "child";
	parent->addChild(child);
	BOOST_CHECK(parent->getNumChildren() == 1);
	BOOST_CHECK(parent->getChildIndex(child) == 0);
	
	for(int i=0; i < 10; i++) {
		poObject* obj = new poObject();
		obj->name = (boost::format("obj%d")%i).str();
		parent->addChild(obj);
	}
	BOOST_CHECK(parent->getNumChildren() == 11);
	BOOST_CHECK(parent->getChildIndex(child) == 0);
	
	parent->moveChildToBack(child);
	BOOST_CHECK(parent->getChildIndex(child) == 0);
	
	parent->moveChildToFront(child);
	BOOST_CHECK(parent->getChildIndex(child) == 10);
	
	while(parent->getNumChildren() > 1) {
		parent->removeChild(0, true);
	}
	BOOST_CHECK(parent->getNumChildren() == 1); // error: there are 6 children instead of 1.
//	printf("child idx: %d\n", parent->getChildIndex(child));
	BOOST_CHECK(parent->getChildIndex(child) == 0);
	
	poObject* after_child = new poObject();
	parent->addChildAfter(after_child, child);
	BOOST_CHECK(parent->getChildIndex(after_child) == (parent->getChildIndex(child) + 1));
	BOOST_CHECK(parent->getLastChild() == after_child);
	
	poObject* before_child = new poObject();
	parent->addChildBefore(before_child, child);
	BOOST_CHECK(parent->getChildIndex(before_child) == (parent->getChildIndex(child) - 1));
	BOOST_CHECK(parent->getLastChild() == after_child);
	
	poObject* moving_child = new poObject();
	parent->addChild(moving_child);
	BOOST_CHECK(parent->getChildIndex(moving_child) == parent->getNumChildren()-1);
	
	int idx = parent->getChildIndex(moving_child);
	parent->moveChildBackward(moving_child);
	BOOST_CHECK(parent->getChildIndex(moving_child) == idx-1); // error: it moved from index 8 to 1...
	
	idx = parent->getChildIndex(moving_child);
	parent->moveChildForward(moving_child); // error: this makes it crash
	BOOST_CHECK(parent->getChildIndex(moving_child) == idx+1); // error: it moved from index 8 to 1...
	
	parent->removeAllChildren(true);
	BOOST_CHECK(parent->getNumChildren() == 0);

}
	
BOOST_AUTO_TEST_CASE( poObject_events ) {
	// EVENTS	
	poObject *parent = new poObject();
	
	parent->addEvent(PO_MOUSE_UP_EVENT, parent, "event1");
	parent->addEvent(PO_MOUSE_UP_EVENT, parent, "event2");
	parent->addEvent(PO_MOUSE_UP_EVENT, parent, "event3");
	parent->addEvent(PO_MOUSE_DOWN_EVENT, parent, "event4");
	parent->addEvent(PO_MOUSE_DOWN_EVENT, parent, "event5");
	parent->addEvent(PO_KEY_DOWN_EVENT, parent, "event6");
	parent->addEvent(PO_KEY_DOWN_EVENT, parent, "event7");
	parent->addEvent(PO_KEY_DOWN_EVENT, parent, "event8");
	parent->addEvent(PO_KEY_DOWN_EVENT, parent, "event9");
	parent->addEvent(PO_TOUCH_BEGAN_EVENT, parent, "event10");
	parent->addEvent(PO_TOUCH_BEGAN_EVENT, parent, "event11");
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_MOUSE_UP_EVENT).size() == 3);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_MOUSE_DOWN_EVENT).size() == 2);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_KEY_DOWN_EVENT).size() == 4);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_TOUCH_BEGAN_EVENT).size() == 2);
	
	parent->removeAllEventsOfType(PO_MOUSE_UP_EVENT);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_UP_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_MOUSE_DOWN_EVENT).size() == 2);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_KEY_DOWN_EVENT).size() == 4);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_TOUCH_BEGAN_EVENT).size() == 2);
	
	parent->removeAllEventsOfType(PO_MOUSE_DOWN_EVENT);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_UP_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_KEY_DOWN_EVENT).size() == 4);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_TOUCH_BEGAN_EVENT).size() == 2);
	
	parent->removeAllEventsOfType(PO_KEY_DOWN_EVENT);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_UP_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_KEY_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(parent, PO_TOUCH_BEGAN_EVENT).size() == 2);
	
	parent->removeAllEventsOfType(PO_TOUCH_BEGAN_EVENT);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_UP_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_MOUSE_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_KEY_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(parent, PO_TOUCH_BEGAN_EVENT) == false);

	poObject *child = new poObject();
	parent->addChild(child);
	
	child->addEvent(PO_MOUSE_UP_EVENT, parent, "event1");
	child->addEvent(PO_MOUSE_UP_EVENT, parent, "event2");
	child->addEvent(PO_MOUSE_UP_EVENT, parent, "event3");
	child->addEvent(PO_MOUSE_DOWN_EVENT, parent, "event4");
	child->addEvent(PO_MOUSE_DOWN_EVENT, parent, "event5");
	child->addEvent(PO_KEY_DOWN_EVENT, parent, "event6");
	child->addEvent(PO_KEY_DOWN_EVENT, parent, "event7");
	child->addEvent(PO_KEY_DOWN_EVENT, parent, "event8");
	child->addEvent(PO_KEY_DOWN_EVENT, parent, "event9");
	child->addEvent(PO_TOUCH_BEGAN_EVENT, parent, "event10");
	child->addEvent(PO_TOUCH_BEGAN_EVENT, parent, "event11");
	BOOST_CHECK(poEventCenter::get()->eventsForObject(child, PO_MOUSE_UP_EVENT).size() == 3);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(child, PO_MOUSE_DOWN_EVENT).size() == 2);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(child, PO_KEY_DOWN_EVENT).size() == 4);
	BOOST_CHECK(poEventCenter::get()->eventsForObject(child, PO_TOUCH_BEGAN_EVENT).size() == 2);
	
	parent->removeAllEvents();
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(child, PO_MOUSE_UP_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(child, PO_MOUSE_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(child, PO_KEY_DOWN_EVENT) == false);
	BOOST_CHECK(poEventCenter::get()->objectHasEvent(child, PO_TOUCH_BEGAN_EVENT) == false);
}
	
BOOST_AUTO_TEST_CASE( poObject_modifiers) {
	// MODIFIERS
	
	poObject *parent = new poObject();
	
	poObjectModifier* modifier = new poObjectModifier();
	BOOST_CHECK(modifier->isEnabled() == true);
	
	parent->addModifier(modifier);
	BOOST_CHECK(modifier->isEnabled() == true);
	BOOST_CHECK(parent->getNumModifiers() == 1);
	
	parent->removeModifier(modifier, false);
	BOOST_CHECK(parent->getNumModifiers() == 0);

	parent->addModifier(modifier);
	for(int i=0; i < 10; i++){
		parent->addModifier(new poObjectModifier());
	}
	BOOST_CHECK(parent->getNumModifiers() == 11);
	BOOST_CHECK(parent->getModifier(0) == modifier);
	
	parent->removeModifier(5);
	BOOST_CHECK(parent->getNumModifiers() == 10);
	BOOST_CHECK(parent->getModifier(0) == modifier);
	
	parent->removeAllModifiers(true);
	BOOST_CHECK(parent->getNumModifiers() == 0);
}
	

BOOST_AUTO_TEST_CASE( poObject_bounds ) {
	// BOUNDS
	
	poRectShape* rect = new poRectShape(200,200);
	BOOST_CHECK(rect->getWidth() == 200);
	BOOST_CHECK(rect->getHeight() == 200);
	
//	parent->addChild(rect);
//	BOOST_CHECK(parent->getWidth() == 200);
//	BOOST_CHECK(parent->getHeight() == 200);
//	
//	poRectShape* rectChild = new poRectShape(500,500);
//	rect->addChild(rectChild);
//	BOOST_CHECK(parent->getWidth() == 500);
//	BOOST_CHECK(parent->getHeight() == 500);
}
