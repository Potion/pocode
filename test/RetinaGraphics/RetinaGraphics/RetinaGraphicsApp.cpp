/*	Created by Steve Varga on 4/27/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "RetinaGraphicsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
RetinaGraphicsApp::RetinaGraphicsApp() {
	addModifier(new poCamera2D(poColor::black));
    
//    bg1 = new poRectShape(new poTexture("Resource/bg2.png"));
//    addChild(bg1);
    
//    bg2 = new poRectShape(new poTexture("Resource/bg2.png"));
//    bg2->visible = false;
//    addChild(bg2);
    
    poObject *shapes = new poObject();
    
    int nCols   = 3;
    int nRows   = 3;
    int size    = 200;
    int spacing = 20;
    for(int i=0; i<nCols; i++) {
        for(int j=0; j<nRows; j++) {
            poRectShape *thisRect = new poRectShape(size, size);
            thisRect->name = "Rect";
            thisRect->fillColor.set255(255, 255, 255);
            
            poTextBox *idText = new poTextBox(size);
            idText->setTextAlignment(PO_ALIGN_CENTER_CENTER);
            idText->setTextSize(36);
            idText->textColor.set255(0, 0, 0);
            idText->setText(poToString((nRows*i) + j));
            idText->setFont(poGetFont("Resource/Arial.ttf"));
            idText->doLayout();
            
            idText->position.y = thisRect->getHeight()/2;
            
            poObject *thisShape = new poObject();
            thisShape->drawBounds = true;
            thisShape->addEvent(PO_TOUCH_BEGAN_INSIDE_EVENT, this);
            thisShape->addEvent(PO_TOUCH_ENDED_EVENT, this);
            
            thisShape->position.set(i*(size + spacing), j*(size + spacing), 0.0f);
            thisShape->addChild(thisRect);
            thisShape->addChild(idText);
            shapes->addChild(thisShape);
        }
    }
    
    shapes->position.set(getWindowWidth()/2 - shapes->getWidth()/2, getWindowHeight()/2 - shapes->getHeight()/2, 0.0f);
    addChild(shapes);
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
    std::cout << "X:" << event->globalPosition.x << std::endl;
    std::cout << "Y:" << event->globalPosition.y << std::endl;
    switch (event->type) {
        case PO_TOUCH_BEGAN_INSIDE_EVENT:
            ((poRectShape*)event->source->getChild("Rect"))->fillColor.set255(255, 0, 0);
            break;
            
        case PO_TOUCH_ENDED_EVENT:
            ((poRectShape*)event->source->getChild("Rect"))->fillColor.set255(255, 255, 255);
            break;
    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RetinaGraphicsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
