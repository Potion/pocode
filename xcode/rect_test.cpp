/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include <boost/test/unit_test.hpp>

#include "poRect.h"

BOOST_AUTO_TEST_CASE(poRectTest) 
{
	poRect rect(0,100,0,100);
	BOOST_CHECK_CLOSE(rect.area(), 10000.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.width(), 100.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.height(), 100.f, 0.0001f);
	
	// this is reversed top ~ bottom
	rect.set(-100,100,100,-100);
	BOOST_CHECK_CLOSE(rect.area(), 40000.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.width(), 200.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.height(), 200.f, 0.0001f);
	
	BOOST_CHECK(rect.contains(90,-90));
	BOOST_CHECK(!rect.contains(110,-90));
	
	rect.set(0,100,0,100);
	rect.include(poRect(0,-100,-100,0));
	BOOST_CHECK_CLOSE(rect.width(), 200.f, 0.0001f);
	
	rect.set(0,1,0,1);
	rect.scale(100.f);
	BOOST_CHECK_CLOSE(rect.width(), 100.f, 0.0001f);
	
	rect.scale(2.f);
	BOOST_CHECK_CLOSE(rect.width(), 200.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.bottomRight().x, 200.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.bottomRight().y, 200.f, 0.0001f);
	
	rect.set(0,10,0,10);
	rect.scale(2.f, poPoint(2,2));
	BOOST_CHECK_CLOSE(rect.width(), 20.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.bottomRight().x, 16.f, 0.0001f);
	BOOST_CHECK_CLOSE(rect.topLeft().y, -4.f, 0.0001f);
}
