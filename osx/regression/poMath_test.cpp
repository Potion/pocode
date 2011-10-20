//
//  poMath_test.cpp
//  potionCode
//
//  Created by Filippo Vanucci on 10/18/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poMath.h"
#include <cfloat>

BOOST_AUTO_TEST_CASE( poMathTest ) {
	
	float f1 = 9495348.943528f;
	float f2 = 9495348.943528f;
	BOOST_CHECK(compare(f1, f2) == 1);
	
	f2 = f1 - 1.f;
	BOOST_CHECK(compare(f1, f2) == 0);
	
	// compare different big numbers
	f1 = 932534534534523234149530.f;
	f2 = 932534534534523234149539.f;
	BOOST_CHECK(compare(f1, f2) == 1); // ??

	float deg = 90.f;
	float rad = deg2rad(deg);
	BOOST_CHECK_CLOSE(rad, M_PI/2, 0.00005f);
	deg = rad2deg(rad);
	BOOST_CHECK_CLOSE(deg, 90.f, 0.00005f);
	
	rad = 1.12838218312f;
	deg = rad2deg(rad);
	rad = deg2rad(deg);
	BOOST_CHECK_CLOSE(rad, 1.12838218312f, 0.00005f);
	
	deg = 0;
	BOOST_CHECK(compare(sin_deg(deg),0.f) == 1);
	deg = 90;
	BOOST_CHECK(compare(sin_deg(deg),1.f) == 1);
	deg = 180;
	BOOST_CHECK(compare(sin_deg(deg),0.f) == 1);
	deg = 270;
	BOOST_CHECK(compare(sin_deg(deg),-1.f) == 1);
	deg = 360;
	BOOST_CHECK(compare(sin_deg(deg),0.f) == 1);
	
	deg = 0;
	BOOST_CHECK(compare(cos_deg(deg),1.f) == 1);
	deg = 90;
	BOOST_CHECK(compare(cos_deg(deg),0.f) == 1);
	deg = 180;
	BOOST_CHECK(compare(cos_deg(deg),-1.f) == 1);
	deg = 270;
	BOOST_CHECK(compare(cos_deg(deg),0.f) == 1);
	deg = 360;
	BOOST_CHECK(compare(cos_deg(deg),1.f) == 1);
	
	f1 = poRand(0.f,0.0001f);
	BOOST_CHECK(f1 >= 0.f && f1 <= 0.0001f);
	f1 = poRand(-19293191919219292834928392.f,19293191919219292834928392.f);
	BOOST_CHECK(f1 >= -19293191919219292834928392.f && f1 <= 19293191919219292834928392.f);
	
	poRect rect(-10000000.f,-10000000.f,99999999.f,99999999.f);
	poPoint randomP(randomPointInRect(rect));
	BOOST_CHECK(rect.contains(randomP) == 1);
	
	poColor randomC = randomColor();
	BOOST_CHECK(randomC.R >= 0.f && randomC.R <= 1.f);
	BOOST_CHECK(randomC.G >= 0.f && randomC.G <= 1.f);
	BOOST_CHECK(randomC.B >= 0.f && randomC.B <= 1.f);
	
	f1 = 9182812783319084134132048712340.f;
	f2 = 2238298392138921839289329389283.f;
	capToRange(&f1, 0.f, 1.f);
	capToRange(&f2, 0.f, 1.f);
	BOOST_CHECK(f1 == 1.f);
	BOOST_CHECK(f1 == f2);
	
	f1 *= -1;
	f2 *= -1;
	capToRange(&f1, 0.f, 1.f);
	capToRange(&f2, 0.f, 1.f);
	BOOST_CHECK(f1 == 0.f);
	BOOST_CHECK(f1 == f2);
	
	f1 = round(f1 + 0.5f);
	BOOST_CHECK(f1 == f2 + 1.f);
	
	f1 = 1000000.000000f;
	f2 = 1000000.000000f;
	f1 = round(f1 + 0.5f);
	BOOST_CHECK(f1 == f2 + 1);
	
	f1 = f2 = 0;
	f1 = roundSym(f1 - 0.5f);
	BOOST_CHECK(f1 == f2 - 1.f);
	
	f1 = -1000000.000000f;
	f2 = -1000000.000000f;
	f1 = roundSym(f1 - 0.5f);
	BOOST_CHECK(f1 == f2 - 1);
	
	poPoint P(1.000001f,1.000001f,1.000001f);
	P = floor(P);
	BOOST_CHECK(P.x == 1.f);
	BOOST_CHECK(P.x == 1.f);
	BOOST_CHECK(P.x == 1.f);
	
	P.set(1.999999f,1.999999f,1.999999f);
	P = floor(P);
	BOOST_CHECK(P.x == 1.f);
	
	// T clamp
	// poMapf
	// poDist
	// poRay
}

