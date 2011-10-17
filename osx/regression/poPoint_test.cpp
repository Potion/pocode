//
//  main.cpp
//  potionCode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#define BOOST_TEST_MODULE poPointTest
#include <boost/test/unit_test.hpp>
#include "poPoint.h"

BOOST_AUTO_TEST_CASE( poPoint_test ) {
	poPoint p1(10,11,12);
	poPoint p2(6,5,4);
	poPoint rez = p1 + p2;
	BOOST_CHECK_CLOSE(rez.x, 16.f, 0.000005f);
	BOOST_CHECK_CLOSE(rez.y, 16.f, 0.000005f);
	BOOST_CHECK_CLOSE(rez.z, 16.f, 0.000005f);
}