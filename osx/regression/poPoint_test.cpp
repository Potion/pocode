//
//  main.cpp
//  potionCode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poPoint.h"

BOOST_AUTO_TEST_CASE( poPoint_test ) {
	
	poPoint p1(10,11,12);
	BOOST_CHECK_CLOSE(p1.x, p1.y, 10.00005f);
	
	poPoint p2;
	p2.set(6,5,4);
	poPoint rez = p1 + p2;
	BOOST_CHECK_CLOSE(rez.x, 16.f, 0.000005f);
	BOOST_CHECK_CLOSE(rez.y, 16.f, 0.000005f);
	BOOST_CHECK_CLOSE(rez.z, 16.f, 0.000005f);
	
	poPoint p3(5.f,5.f,2.5f);
	
	p3.normalize();
	float l = p3.getLength();
	BOOST_CHECK_CLOSE(l, 1.f, 0.000010f);
	
	float lsquared = p1.getLength() * p1.getLength();
	BOOST_CHECK_CLOSE(lsquared, p1.getLengthSquared(), 0.000005f);
	
	poPoint p4(10,10,0);
	poPoint p5;
	p5.setPolar(45, p4.getLength());
	BOOST_CHECK_CLOSE(p4.x, p5.x, 0.000005f);
	BOOST_CHECK_CLOSE(p4.y, p5.y, 0.000005f);
	
	float dist = p4.getDist(p5);
	BOOST_CHECK_CLOSE(dist, 0.f, 0.000005f);
	
	p4.set(110,10,0);
	dist = p4.getDist(p5);
	BOOST_CHECK_CLOSE(dist, 100.f, 0.000005f);
	
	std::string str = p4.toString();
	BOOST_CHECK(str == "point(110,10,0)");
	
	p4 = p5;
	BOOST_CHECK(p4.getDot(p5) == p4.getLengthSquared());
	
	p4.set(10,10,10);
	p5 = p4 + poPoint(100,100,100);
	BOOST_CHECK_CLOSE(p5.x, 110.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.y, 110.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.z, 110.f, 0.000005f);
	
	p5 = p4 - poPoint(10,10,10);
	BOOST_CHECK_CLOSE(p5.x, 0.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.y, 0.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.z, 0.f, 0.000005f);
	
	p5 = p4 * 11;
	BOOST_CHECK_CLOSE(p5.x, 110.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.y, 110.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.z, 110.f, 0.000005f);
	
	p5 = p4 / 10;
	BOOST_CHECK_CLOSE(p5.x, 1.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.y, 1.f, 0.000005f);
	BOOST_CHECK_CLOSE(p5.z, 1.f, 0.000005f);
	
	p4.set(10,0,0);
	p5.set(0,10,0);
	poPoint cross = p4.getCross(p5);
	BOOST_CHECK_CLOSE(cross.x, 0.f, 0.000005f);
	BOOST_CHECK_CLOSE(cross.y, 0.f, 0.000005f);
	BOOST_CHECK_CLOSE(cross.z, 100.f, 0.000005f);
	
	poPoint norm2D(p4.getNormal2D());
	BOOST_CHECK_CLOSE(norm2D.x, 0.f, 0.000005f);
	BOOST_CHECK_CLOSE(norm2D.y, -10.f, 0.000005f);
	BOOST_CHECK_CLOSE(norm2D.z, 0.f, 0.000005f);
	
	poPoint rotp4(p4.getRotate2D(90.f));
	BOOST_CHECK(rotp4.x > -0.000001f && rotp4.x < 0.000001f);
	BOOST_CHECK(rotp4.y > 9.999999f && rotp4.y < 10.000001f);
	BOOST_CHECK(rotp4.z > -0.000001f && rotp4.z < 0.000001f);
}
