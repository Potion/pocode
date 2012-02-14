/*	Created by filippo on 2/13/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "layoutModifierApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include"poTextBox.h"


// APP CONSTRUCTOR. Create all objects here.
layoutModifierApp::layoutModifierApp() {
	addModifier(new poCamera2D(poColor::white));
	
	
	obj = new poObject();
	obj->drawBounds = true;
	obj->position.set(200,200,0);
	addChild(obj);
	
	poLayout* layout = new poLayout(PO_VERTICAL);
	layout->setSpacing(20,10);
	obj->addModifier(layout);
	
	poRandSeed();
	
	poPoint pos;
	for(int i=0; i < 10; i++) {
		
		if(i % 2 == 0) {
			poRectShape* shape = new poRectShape(poRand(20, 100), poRand(20, 80));
			shape->alpha = 0.7;
			shape->fillColor = poColor::random();
			shape->setAlignment(poAlignment(int(poRand(0,9))));
			obj->addChild(shape);
		}
		else {
			poTextBox* TB = new poTextBox(); //poRand(80, 130));
			TB->setFont(poGetFont("Helvetica", "Bold"));
			TB->setText("Provaprova un due tre prova si no boh");
			TB->setTextSize(int(poRand(12,30)));
			TB->textColor = poColor::black;
			TB->drawBounds = true;
			TB->useTextBoundsAsBounds(true);
			TB->setAlignment(poAlignment(int(poRand(0,9))));
			TB->doLayout();
			obj->addChild(TB);
		}
	}
	
	counter = 0;
	
//	obj->setAlignment(PO_ALIGN_CENTER_CENTER); // WEIRD!!!!
}

// APP DESTRUCTOR. Delete all objects here.
layoutModifierApp::~layoutModifierApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void layoutModifierApp::update() {
//	obj->setAlignment(PO_ALIGN_CENTER_CENTER); // WEIRD!!!!
}

// DRAW. Called once per frame. Draw objects here.
void layoutModifierApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void layoutModifierApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void layoutModifierApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
