/*	Created by Steve Varga on 9/14/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poApplicationNamespaceTestApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
poApplicationNamespaceTestApp::poApplicationNamespaceTestApp() {
	addModifier(new poCamera2D(poColor::black));
}

// APP DESTRUCTOR. Delete all objects here.
poApplicationNamespaceTestApp::~poApplicationNamespaceTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poApplicationNamespaceTestApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void poApplicationNamespaceTestApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poApplicationNamespaceTestApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poApplicationNamespaceTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
