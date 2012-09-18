/*	Created by Joshua Fisher on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "fontTestApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poOpenGLState.h"
#include "poShapeBasics2D.h"

// APP CONSTRUCTOR. Create all objects here.
fontTestApp::fontTestApp() {
	addModifier(new poCamera2D(poColor::green));
	
	addChild(new poRectShape("test1.png"));
	addChild(new poRectShape("test2.png"));
	
	
	poTextBox *tb = new poTextBox(300);
	tb->setText("hello world");
	tb->setFont(poGetFont("/System/Library/Fonts/Helvetica.dfont", "Regular"));
	tb->textColor = poColor::yellow;
	tb->setTextSize(30);
	tb->doLayout();
	addChild(tb);
}

// APP DESTRUCTOR. Delete all objects here.
fontTestApp::~fontTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void fontTestApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void fontTestApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void fontTestApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void fontTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
