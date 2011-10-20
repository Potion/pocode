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
#include "poApplication.h"
#include "Helpers.h"
#include "poResourceLoader.h"
#include "poGeometryMask.h"

poObject *createObjectForID(uint id) {
	return new TestObj();
}

TestObj::TestObj() {
	addModifier(new poCamera2D(poColor::white));
	
	for(int i=0; i<100; i++) {
		poTextBox *text = addChild(new poTextBox(200));
		text->font(getFont("Maharam-Regular.otf",""));
		text->textSize(poRand(12,20));
		text->textColor = poColor::red;
		text->text(std::string("hello:") + boost::lexical_cast<std::string>(text->textSize()));
		text->leading(.7f);
		text->position = randomPointInRect(getWindowFrame());
		text->layout();
	}
}

void TestObj::update() {
	for(int i=0; i<100; i++) {
		poTextBox *text = getChildAs<poTextBox>(i);
	}
}

void TestObj::eventHandler(poEvent *event) {
}


