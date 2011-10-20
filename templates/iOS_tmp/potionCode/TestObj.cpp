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
	addModifier(new poCamera2D());
	
	for(int i=0; i<100; i++) {
		poTextBox *text = addChild(new poTextBox(200));
		text->font(getFont("Maharam-Regular.otf",""));
		text->textSize(50);
		text->textColor = poColor::red;
		text->text("hello world!!");
		text->leading(.7f);
		text->position = randomPointInRect(getWindowFrame());
		text->layout();
	}
	
}

void TestObj::update() {
	for(int i=0; i<100; i++) {
		poTextBox *text = getChildAs<poTextBox>(i);
		text->rotation += 1.f;
	}
}

void TestObj::eventHandler(poEvent *event) {
}


