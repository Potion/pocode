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
	setCurrentPath("resource");
	return new TestObj();
}

TestObj::TestObj() {
	addModifier(new poCamera2D(poColor::black));
	
	poTextBox *text = addChild(new poTextBox(150));
	text->font(getFont("Maharam-Regular.otf",""));
	text->textSize(15);
	text->leading(.5f);
	text->textColor = poColor::white;
	text->text("quick brown fox jumps over the lazy dog");
	text->textColor = poColor::white;
	text->position.set(100,100,0);
//	text->drawBounds = PO_TEXT_BOX_STROKE_TEXT_BOUNDS;
	text->layout();
	
	text = addChild((poTextBox*)text->copy());
	text->position.y += text->textBounds().height+10;
	text->cacheToTexture(false);
	text->layout();
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}


