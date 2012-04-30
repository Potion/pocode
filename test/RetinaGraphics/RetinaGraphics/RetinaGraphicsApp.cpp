/*	Created by Steve Varga on 4/27/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "RetinaGraphicsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
RetinaGraphicsApp::RetinaGraphicsApp() {
	addModifier(new poCamera2D(poColor::black));
    
    bg1 = new poRectShape(new poTexture("Resource/bg2.png"));
    addChild(bg1);
    
//    bg2 = new poRectShape(new poTexture("Resource/bg2.png"));
//    bg2->visible = false;
//    addChild(bg2);
    
    poRectShape *square = new poRectShape(100,100);
    square->fillColor.set255(0, 0, 0);
    
    poOvalShape *circle = new poOvalShape(200, 200, 200);
    circle->fillColor.set(0,0,0);
    circle->position.x = 200;
    
    poObject *shapes = new poObject();
    shapes->addChild(square);
    shapes->addChild(circle);
    shapes->position.x = getWindowWidth()/2 - shapes->getWidth()/2;
    addChild(shapes);
    
    text = new poTextBox(300);
    text->setFont(poGetFont("Resource/Arial.ttf"));
    text->setText("Test Text!");
    text->setTextSize(72);
    text->textColor.set(0.0f, 0.0f, 0.f);
    text->doLayout();
    text->position.set(getWindowWidth()/2, getWindowHeight()/2, 0.0f);
    addChild(text);
    
    addEvent(PO_TOUCH_BEGAN_EVENT, this);
    addEvent(PO_TOUCH_MOVED_EVENT, this);
}

// APP DESTRUCTOR. Delete all objects here.
RetinaGraphicsApp::~RetinaGraphicsApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void RetinaGraphicsApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void RetinaGraphicsApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void RetinaGraphicsApp::eventHandler(poEvent *event) {
//    if(event->type == PO_TOUCH_BEGAN_EVENT) {
//        if(bg1->visible) {
//            bg1->visible = false;
//            bg2->visible = true;
//        } else {
//            bg1->visible = true;
//            bg2->visible = false;
//        }
//    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RetinaGraphicsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
