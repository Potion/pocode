/*	Created by Stephen Varga on 9/18/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "RichtTextCachingApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
RichtTextCachingApp::RichtTextCachingApp() {
	addModifier(new po::Camera2D(po::Color::black));
}

// APP DESTRUCTOR. Delete all objects here.
RichtTextCachingApp::~RichtTextCachingApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void RichtTextCachingApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void RichtTextCachingApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void RichtTextCachingApp::eventHandler(po::Event *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RichtTextCachingApp::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender) {
	
}
