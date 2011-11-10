//
//  poRect_test.cpp
//  potionCode
//
//  Created by Filippo Vanucci on 10/17/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poRect.h"

BOOST_AUTO_TEST_CASE( poRectTest ) {
	
	poRect r1(10,10,25,25);
	BOOST_CHECK_CLOSE(r1.x, r1.y, 0.00005f);
	BOOST_CHECK_CLOSE(r1.width, r1.height, 0.00005f);
	
	poRect r2(poPoint(10,10),poPoint(25,25));
	BOOST_CHECK_CLOSE(r1.x, r2.x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.y, r2.y, 0.00005f);
	BOOST_CHECK_CLOSE(r1.width, r2.width, 0.00005f);
	BOOST_CHECK_CLOSE(r1.height, r2.height, 0.00005f);
	
	r1.set(poPoint(111.1f,111.1f),poPoint(999.9f,555.9f));
	r2.fromString("rect(111.1,111.1,999.9,555.9)");
	BOOST_CHECK_CLOSE(r1.x, r2.x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.y, r2.y, 0.00005f);
	BOOST_CHECK_CLOSE(r1.width, r2.width, 0.00005f);
	BOOST_CHECK_CLOSE(r1.height, r2.height, 0.00005f);
	
	BOOST_CHECK(r1.contains(111.1f,111.1f));
	BOOST_CHECK(r1.contains(poPoint((111.1f + 999.9f)/2,(111.1f + 555.9f)/2)));
	BOOST_CHECK(r1.contains(999.9f,555.9f));
	
	BOOST_CHECK(r1.toString() == "rect(111.1,111.1,999.9,555.9)");
	
	r1.scale(2.f);
	BOOST_CHECK_CLOSE(r1.width, r2.width * 2, 0.00005f);
	BOOST_CHECK_CLOSE(r1.height, r2.height * 2, 0.00005f);
	BOOST_CHECK_CLOSE(r1.x, r2.x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.y, r2.y, 0.00005f);
	BOOST_CHECK(r1.getArea() != r2.getArea());
	BOOST_CHECK(r1.getAspect() == r2.getAspect());
	r1.scale(0.25f);
	BOOST_CHECK_CLOSE(r1.width, r2.getSize().x / 2, 0.00005f);
	BOOST_CHECK_CLOSE(r1.height, r2.getSize().y / 2, 0.00005f);
	BOOST_CHECK_CLOSE(r1.x, r2.getPosition().x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.y, r2.getPosition().y, 0.00005f);
	BOOST_CHECK(r1.getArea() != r2.getArea());
	BOOST_CHECK(r1.getAspect() == r2.getAspect());
	r1.scale(2.f);
	BOOST_CHECK_CLOSE(r1.width, r2.getSize().x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.height, r2.getSize().y, 0.00005f);
	BOOST_CHECK_CLOSE(r1.x, r2.getPosition().x, 0.00005f);
	BOOST_CHECK_CLOSE(r1.y, r2.getPosition().y, 0.00005f);
	BOOST_CHECK(r1.getArea() == r2.getArea());
	BOOST_CHECK(r1.getAspect() == r2.getAspect());
	
	r1.scale(999.f, r1.getCenter());
	BOOST_CHECK(r1.x < 0.f);
	BOOST_CHECK(r1.y < 0.f);
	BOOST_CHECK_CLOSE(r1.getCenter().x, r2.getCenter().x, 0.05f);
	BOOST_CHECK_CLOSE(r1.getCenter().y, r2.getCenter().y, 0.05f);
	BOOST_CHECK(r1.getAspect() == r2.getAspect());
	
	poRect inc = r1;
	inc.include(r2);
	BOOST_CHECK(r1.getArea() == inc.getArea());
	
	r2.setPosition(poPoint(0,0));
	r2.setSize(poPoint(1000000,100000));
	inc.include(r2);
	BOOST_CHECK(r1.getArea() != inc.getArea());
	
	r1.setPosition(-111.1f,-111.1f);
	r1.setSize(555.9f,555.9f);
	r2.setPosition(poPoint(-111.1f,-111.1f));
	r2.setSize(poPoint(555.9f,555.9f));
	
	r1.include(poPoint(-111.1f,-111.1f));
	r1.include(555.9f,555.9f);
	r1.include(r2);
	
	// test remap
}
