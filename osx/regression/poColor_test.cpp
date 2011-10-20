//
//  poColor_test.cpp
//  potionCode
//
//  Created by Filippo Vanucci on 10/18/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poColor.h"

BOOST_AUTO_TEST_CASE( poColorTest ) {
	
	poColor c1(1.f,1.f,1.f,1.f);
	poColor c2;
	c2.set(1.f,1.f,1.f);
	BOOST_CHECK(c1 == c2);
	
	
}
