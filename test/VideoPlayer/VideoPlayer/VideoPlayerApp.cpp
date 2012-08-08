/*	Created by Joshua Fisher on 8/8/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "VideoPlayerApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poVideo.h"


// APP CONSTRUCTOR. Create all objects here.
VideoPlayerApp::VideoPlayerApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poVideoPlayer *player = new poVideoPlayer("test.mov");
	player->play();
	
	addChild(player);
}

// APP DESTRUCTOR. Delete all objects here.
VideoPlayerApp::~VideoPlayerApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void VideoPlayerApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void VideoPlayerApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void VideoPlayerApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void VideoPlayerApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
