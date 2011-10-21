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
	
	holder = addChild(new poObject());
	holder = holder->addChild(new poObject());
	holder = holder->addChild(new poObject());
	holder = holder->addChild(new poObject());
	
	for(int i=0; i<100; i++) {
		poShape2D *shp = holder->addChild(new poRectShape(100,100));
		shp->alpha = poMapf(0, 100, i, .2, .7);
		shp->position = randomPointInRect(getWindowFrame());
		shp->placeTexture(poTexture(getImage("apple.jpg")), PO_TEX_FIT_INSIDE);
		shp->fillColor = poColor::orange;
		shp->generateStroke(3);
	}
	
	for(int i=0; i<100; i++) {
		poTextBox *text = holder->addChild(new poTextBox(150));
		text->font(getFont("Maharam-Regular.otf",""));
		text->textSize(poRand(25,50));
		text->textColor = poColor::white;
		text->text(std::string("hello:") + boost::lexical_cast<std::string>(text->textSize()));
		text->textAlignment(PO_ALIGN_CENTER);
		text->leading(.7f);
		text->scale.set(.5,.5,1.f);
		text->position = randomPointInRect(getWindowFrame());
		text->layout();
	}
	
	poShape2D *mask = new poOvalShape(500,500,20);
	mask->position = getWindowCenter();
	addModifier(new poGeometryMask(mask));
}

void TestObj::update() {
	holder->alpha = poMapf(-1,1, sin(getTime()), .5,1);
	
	for(int i=0; i<200; i++) {
		poObject *obj = holder->getChild(i);
		obj->rotation += (i<100 ? -1.f : 1.f);
	}
}

void TestObj::eventHandler(poEvent *event) {
}


