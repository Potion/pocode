//
//  TestObj.cpp
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "TestObj.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint id) {
	return new TestObj();
}

TestObj::TestObj() {
	
	addModifier(new poCamera2D(poColor::green));
    
    poRectShape *testRect = addChild(new poRectShape(50,50));
    testRect->fillColor.set255(255,0,0);
    testRect->addEvent(PO_TOUCH_MOVED_EVERYWHERE_EVENT, this);
}


void TestObj::eventHandler(poEvent *event) {
    std::cout << "Touch event!" << std::endl;
}