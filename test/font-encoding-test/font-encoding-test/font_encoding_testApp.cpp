/*	Created by Joshua Fisher on 4/10/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "font_encoding_testApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poFont.h"

typedef std::vector<std::string> string_vec;

// APP CONSTRUCTOR. Create all objects here.
font_encoding_testApp::font_encoding_testApp() {
	addModifier(new poCamera2D(poColor::black));

	poFont *font = poGetFont("Helvetica");
	string_vec encodings = font->getEncodings();
	for(string_vec::iterator iter=encodings.begin(); iter!=encodings.end(); iter++) {
		printf("%s\n", iter->c_str());
	}
	
}

// APP DESTRUCTOR. Delete all objects here.
font_encoding_testApp::~font_encoding_testApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void font_encoding_testApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void font_encoding_testApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void font_encoding_testApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void font_encoding_testApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
