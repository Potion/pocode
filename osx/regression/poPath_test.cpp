//
//  poPath_test.cpp
//  potionCode
//
//  Created by Joshua Fisher on 11/11/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poPath.h"

BOOST_AUTO_TEST_CASE( poPath_linear ) {
	poPath path;
	path.start(0,0);
	path.lineTo(100,0);
	path.lineTo(100,100);
	path.lineTo(0,100);
	path.close();
	
	std::vector<poPoint> points = path.generatePoints();
	BOOST_CHECK(points.size() == 5);
	BOOST_CHECK_CLOSE(points[0].x, 0.f, 0.00001);
	BOOST_CHECK_CLOSE(points[1].y, 0.f, 0.00001);
	BOOST_CHECK_CLOSE(points[2].x, 100.f, 0.00001);
	BOOST_CHECK_CLOSE(points[3].y, 100.f, 0.00001);
	BOOST_CHECK_CLOSE(points[4].x, 0.f, 0.00001);
}
