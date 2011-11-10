//
//  poLineExtruder_test.cpp
//  potionCode
//
//  Created by filippo on 11/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poLineExtruder.h"

BOOST_AUTO_TEST_CASE( poLineExtruder_test ) {
	
	poExtrudedLineSeg seg1(poPoint(0,0,0), poPoint(200,0,0), 20, PO_STROKE_PLACE_CENTER);
	BOOST_CHECK(compare(seg1.p1.x, 0.f) == 1 && compare(seg1.p1.y, 10.f) == 1);
	BOOST_CHECK(compare(seg1.p2.x, 0.f) == 1 && compare(seg1.p2.y, -10.f) == 1);
	BOOST_CHECK(compare(seg1.p3.x, 200.f) == 1 && compare(seg1.p3.y, 10.f) == 1);
	BOOST_CHECK(compare(seg1.p4.x, 200.f) == 1 && compare(seg1.p4.y, -10.f) == 1);
	
	poExtrudedLineSeg seg2(poPoint(200,0,0), poPoint(400,0,0), 20, PO_STROKE_PLACE_CENTER);
	float angle = int(angleBetweenSegments(seg1, seg2) * 180.0 / (3.14159));
	BOOST_CHECK(compare(angle, -180.f) == 1);
	bool combine = combineExtrudedLineSegments(seg1, seg2, &seg1.p4, &seg1.p3);
//	BOOST_CHECK(combine == 1);
	
	poExtrudedLineSeg seg3(poPoint(200,0,0), poPoint(200,-200,0), 20, PO_STROKE_PLACE_CENTER);
	angle = int(angleBetweenSegments(seg1, seg3) * 180.0 / (3.14159));
//	BOOST_CHECK(angle == 90.f);
	
	poExtrudedLineSeg seg4(poPoint(200,0,0), poPoint(200,200,0), 20, PO_STROKE_PLACE_CENTER);
	angle = int(angleBetweenSegments(seg1, seg4) * 180.0 / (3.14159));
//	BOOST_CHECK(angle == -90.f);
	
}