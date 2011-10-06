//
//  TestObj.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poObject.h"
#include "poShape2D.h"
#include "poTextBox.h"

class TestObj : public poObject {
public:
	TestObj();
	
	void update();
    void eventHandler(poEvent *event);
	
	poShape2D *shape;
	poTextBox *text;
};