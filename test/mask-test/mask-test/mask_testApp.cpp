/*	Created by Joshua Fisher on 9/11/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "mask_testApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poGeometryMask.h"


// APP CONSTRUCTOR. Create all objects here.
mask_testApp::mask_testApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poShape2D *shp = new poOvalShape(getWindowWidth()/2);
	poShape2D *mask = new poRectShape(getWindowWidth()/2,getWindowHeight()/2);
	shp->addModifier(new poGeometryMask(mask));
	shp->position = getWindowCenter();
	shp->rotationTween.set(360.f).setTweenFunction(PO_TWEEN_LINEAR_FUNC).setDuration(5.f).setRepeat(PO_TWEEN_REPEAT_REGULAR).start();
	addChild(shp);
}

// APP DESTRUCTOR. Delete all objects here.
mask_testApp::~mask_testApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void mask_testApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void mask_testApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void mask_testApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void mask_testApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
