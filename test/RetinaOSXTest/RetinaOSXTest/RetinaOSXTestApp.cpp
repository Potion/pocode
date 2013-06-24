/*	Created by Stephen Varga on 4/30/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "RetinaOSXTestApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2d.h"

// APP CONSTRUCTOR. Create all objects here.
RetinaOSXTestApp::RetinaOSXTestApp() {
	addModifier(new po::Camera2D(po::Color::black));
    
    po::TextBox *test = new po::TextBox(1000);
    test->setTextSize(10);
    test->setText("Testing retina text!");
    test->useTextBoundsAsBounds(true);
    test->doLayout();
    
    addChild(test);
    
    po::RectShape * r = new po::RectShape(100, 100);
    r->fillColor.set(1, 0, 0);
    addChild(r);
    moveChildBackward(r);
    
    std::cout << "Scale: " << po::getScale() << std::endl;
}

// APP DESTRUCTOR. Delete all objects here.
RetinaOSXTestApp::~RetinaOSXTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void RetinaOSXTestApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void RetinaOSXTestApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void RetinaOSXTestApp::eventHandler(po::Event *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RetinaOSXTestApp::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender) {
	
}
