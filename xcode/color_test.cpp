/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include <boost/test/unit_test.hpp>

#include "poColor.h"
#include "poMath.h"

BOOST_AUTO_TEST_CASE(poColorTest) 
{
	// make sure the basics work
	poColor white(1,1,1);
	BOOST_REQUIRE(white.red == 1 && 
				  white.green == 1 && 
				  white.blue == 1 && 
				  white.alpha == 1);
	
	poColor &black = white.set(0,0,0);
	BOOST_REQUIRE(black.red == 0 &&
				  black.green == 0 &&
				  black.blue == 0 && 
				  black.alpha == 1);
	BOOST_REQUIRE(&black == &white);
	
	HSVColor hsv;
	
	// i'm willing to accept a decent amount of inaccuracy for these conversions
	float accuracy_pct = 2.f;
	
	hsv = rgba2hsv(poColor(1,0,0,1));
	BOOST_CHECK_CLOSE(hsv.hue, 0, accuracy_pct);
	BOOST_CHECK_CLOSE(hsv.saturation, 1.f, accuracy_pct);
	BOOST_CHECK_CLOSE(hsv.value, 1.f, accuracy_pct);
	
	hsv = rgba2hsv(poColor(0,1,1,1));
	BOOST_CHECK_CLOSE(hsv.hue, 180.f, accuracy_pct);
	BOOST_CHECK_CLOSE(hsv.saturation, 1.f, accuracy_pct);
	BOOST_CHECK_CLOSE(hsv.value, 1.f, accuracy_pct);
	
	poColor color;
	color = hsv2rgba(HSVColor(200, .5, .5));
	BOOST_CHECK_CLOSE(color.red, 63.f/255.f, accuracy_pct);
	BOOST_CHECK_CLOSE(color.green, 106.f/255.f, accuracy_pct);
	BOOST_CHECK_CLOSE(color.blue, 127.f/255.f, accuracy_pct);
}
