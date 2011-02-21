/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */


#include <boost/test/unit_test.hpp>

#include "poMath.h"

BOOST_AUTO_TEST_CASE(poMathTest) 
{
	BOOST_CHECK(almostEqual(0.0001f, 0.0005f, 0.0005f));
	BOOST_CHECK(!almostEqual(0.0001f, 0.0005f, 0.0004f));
	BOOST_CHECK_CLOSE(rad2deg(M_PI), 180.f, 0.00001f);
	BOOST_CHECK_CLOSE(deg2rad(180.f), M_PI, 0.00001f);
	BOOST_CHECK_CLOSE(deg2rad(90.f), M_HALFPI, 0.00001f);
	BOOST_CHECK_CLOSE(sin_deg(90.f), 1.f, 0.00001f);
	BOOST_CHECK_CLOSE(cos_deg(180.f), -1.f, 0.00001f);
	BOOST_CHECK_SMALL(cos_deg(90.f), 0.00001f);
}
