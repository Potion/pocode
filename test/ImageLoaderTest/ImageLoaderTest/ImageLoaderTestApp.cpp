/*	Created by Stephen Varga on 9/4/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "ImageLoaderTestApp.h"
#include "poApplication.h"
#include "poCamera.h"

// APP CONSTRUCTOR. Create all objects here.
ImageLoaderTestApp::ImageLoaderTestApp() {
	addModifier(new poCamera2D(poColor::black));
}

// APP DESTRUCTOR. Delete all objects here.
ImageLoaderTestApp::~ImageLoaderTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void ImageLoaderTestApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void ImageLoaderTestApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void ImageLoaderTestApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void ImageLoaderTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
