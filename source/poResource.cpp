//
//  poResource.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poResource.h"

poResourceStore::poResourceStore() {}

poResourceStore::~poResourceStore() {
	BOOST_FOREACH(poResource *r, resources) {
		delete r;
	}
}
