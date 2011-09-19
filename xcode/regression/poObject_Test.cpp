#include <boost/test/unit_test.hpp>

#include "poObject.h"

BOOST_AUTO_TEST_CASE( poObject_test_1 ) {
	poObject *obj = new poObject();
	
	poObject *child;
	obj->addChild(new poObject());
	obj->addChild(new poObject());
	child = obj->addChild(new poObject());
	
	BOOST_CHECK(obj->numChildren() == 3);
	
	BOOST_CHECK(obj->removeChild(child));
	delete child;
	
	BOOST_CHECK(obj->numChildren() == 2);

	obj->removeAllChildren(true);
	BOOST_CHECK(obj->numChildren() == 0);
}