/*	Created by Joshua Fisher on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "fullscreenTestApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR. Create all objects here.
fullscreenTestApp::fullscreenTestApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poRect r = getWindowBounds();
	
	poShape2D *shp = new poOvalShape(50);
	shp->position.set(60, r.height/2, 0);
	shp->positionTween.set(poPoint(r.width-60,r.height/2,0)).setDuration(4).setTweenFunction(PO_TWEEN_LINEAR_FUNC).setRepeat(PO_TWEEN_REPEAT_PINGPONG).start();
	addChild(shp);
}

// APP DESTRUCTOR. Delete all objects here.
fullscreenTestApp::~fullscreenTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void fullscreenTestApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void fullscreenTestApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void fullscreenTestApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void fullscreenTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
