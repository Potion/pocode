/*	Created by Steve Varga on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poAsyncImageLoaderExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
poAsyncImageLoaderExampleApp::poAsyncImageLoaderExampleApp() {
	addModifier(new poCamera2D(poColor::black));
}

// APP DESTRUCTOR. Delete all objects here.
poAsyncImageLoaderExampleApp::~poAsyncImageLoaderExampleApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poAsyncImageLoaderExampleApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void poAsyncImageLoaderExampleApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poAsyncImageLoaderExampleApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poAsyncImageLoaderExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
