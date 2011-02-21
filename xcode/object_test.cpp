/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include <boost/test/unit_test.hpp>

#include "poObject.h"

BOOST_AUTO_TEST_CASE(poObjectTest) 
{
	poObject *root = new poObject("hello obj");
	BOOST_CHECK_EQUAL(root->name, "hello obj");
	
	root->addChild(new poObject("obj 1"));
	root->addChild(new poObject("obj 2"));
	root->addChild(new poObject("obj 3"));
	BOOST_CHECK_EQUAL(root->numChildren(), 3);
	
	root->removeChild(2);
	BOOST_CHECK_EQUAL(root->numChildren(), 2);
	
	poObject *obj;

	obj = root->getChild(2);
	BOOST_CHECK(obj == NULL);
	
	root->addChild(new poObject("obj 4"));
	obj = root->getChild("obj 2");
	BOOST_REQUIRE(obj != NULL);
	BOOST_CHECK_EQUAL("obj 2", obj->name);
	BOOST_CHECK_EQUAL(root->getChildIndex(obj), 1);
	
	root->moveChildToFront(obj);
	BOOST_CHECK_EQUAL(root->getChildIndex(obj), 0);
	root->moveChildToBack(obj);
	BOOST_CHECK_EQUAL(root->getChildIndex(obj), 2);
	root->moveChildForward(obj);
	BOOST_CHECK_EQUAL(root->getChildIndex(obj), 1);
	root->moveChildBackward(obj);
	BOOST_CHECK_EQUAL(root->getChildIndex(obj), 2);
	
	delete root;
	// obj should have deleted too
}
