/*	Created by Steve Varga on 8/22/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poThreadCenterExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "BasicWorker.h"

// APP CONSTRUCTOR. Create all objects here.
poThreadCenterExampleApp::poThreadCenterExampleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    poThreadCenter::get()->addItem(new BasicWorker(), this, "Basic Worker has finished!!!!"); 
}

// APP DESTRUCTOR. Delete all objects here.
poThreadCenterExampleApp::~poThreadCenterExampleApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poThreadCenterExampleApp::update() {
    poThreadCenter::get()->update(); //This is ghetto, right now it has to be called manually, we need to get some sort of hook in to update()
}

// DRAW. Called once per frame. Draw objects here.
void poThreadCenterExampleApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poThreadCenterExampleApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poThreadCenterExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
    std::cout << "Message received from thread center (worker complete message): " << msg << std::endl;
}
