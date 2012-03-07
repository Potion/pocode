/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poMessageCenterExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poMessageCenter.h"
#include <iostream.h>

// APP CONSTRUCTOR. Create all objects here.
poMessageCenterExampleApp::poMessageCenterExampleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    addEvent(PO_MOUSE_DOWN_EVENT, this);
    
    s = new Square();
    s->position.set(getWindowWidth()/2 - s->getWidth()/2, getWindowHeight()/2 - s->getHeight()/2, 0.0f);
    addChild(s);
}

// APP DESTRUCTOR. Delete all objects here.
poMessageCenterExampleApp::~poMessageCenterExampleApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poMessageCenterExampleApp::update() {
    poMessageCenter::get()->update();
    
    
}

// DRAW. Called once per frame. Draw objects here.
void poMessageCenterExampleApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poMessageCenterExampleApp::eventHandler(poEvent *event) {
    poMessageCenter::get()->broadcastMessage(ToggleSquareMessage, this);
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poMessageCenterExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
