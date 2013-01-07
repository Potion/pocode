/*	Created by Steve Varga on 4/27/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "RetinaGraphicsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
RetinaGraphicsApp::RetinaGraphicsApp() {
	addModifier(new po::Camera2D(po::Color::black));
    
    bg1 = new po::RectShape(new po::Texture("Resource/bg2.png"));
    addChild(bg1);
//
//    bg2 = new po::RectShape(new po::Texture("Resource/bg2.png"));
//    bg2->visible = false;
//    addChild(bg2);
    
    po::Object *shapes = new po::Object();
    
    int nCols   = 3;
    int nRows   = 3;
    int size    = 200;
    int spacing = 20;
    for(int i=0; i<nCols; i++) {
        for(int j=0; j<nRows; j++) {
            po::RectShape *thisRect = new po::RectShape(size, size);
            thisRect->name = "Rect";
            thisRect->fillColor.set255(255, 255, 255);
            
            po::TextBox *idText = new po::TextBox(size);
            idText->setTextAlignment(po::ALIGN_CENTER_CENTER);
            idText->setTextSize(36);
            idText->setCacheToTexture(true);
            idText->textColor.set255(0, 0, 0);
            idText->setText(po::toString((nRows*i) + j));
            idText->setFont(po::getFont("Resource/Arial.ttf"));
            idText->doLayout();
            
            idText->position.y = thisRect->getHeight()/2;
            
            po::Object *thisShape = new po::Object();
            thisShape->drawBounds = true;
            thisShape->addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
            thisShape->addEvent(po::TOUCH_ENDED_EVENT, this);
            
            thisShape->position.set(i*(size + spacing), j*(size + spacing), 0.0f);
            thisShape->addChild(thisRect);
            thisShape->addChild(idText);
            shapes->addChild(thisShape);
        }
    }
    
    shapes->position.set(po::getWindowWidth()/2 - shapes->getWidth()/2, po::getWindowHeight()/2 - shapes->getHeight()/2, 0.0f);
    addChild(shapes);
    
    po::TextBox *testText = new po::TextBox(800);
    
    testText->setTextSize(18);
    testText->setCacheToTexture(true);
    testText->textColor.set255(255, 255, 255);
    testText->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam auctor neque id ante tincidunt vestibulum. In hac habitasse platea dictumst. Nunc dictum erat eget ipsum convallis vel laoreet metus bibendum. Fusce odio massa, euismod vitae porta eget, pulvinar ut felis. Duis at orci lacus, nec rutrum nisi. Praesent congue ullamcorper purus, a semper metus tempus id. Vestibulum sit amet tellus lorem, ut sollicitudin arcu. Duis id arcu quis augue tristique pretium. Morbi sed nunc nisl. Nunc consectetur, nisl id tristique aliquam, lacus augue cursus augue, id fermentum dolor arcu quis lectus. Phasellus vulputate imperdiet risus non dignissim.");
    testText->setFont(po::getFont("Resource/Arial.ttf"));
    testText->doLayout();
    
    addChild(testText);
    
    po::RectShape *r = new po::RectShape(po::getWindowWidth(), 2);
    r->fillColor.set255(255,0,0);
    r->position.y = 19;
    addChild(r);
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
void RetinaGraphicsApp::eventHandler(po::Event *event) {
//    std::cout << "X:" << event->globalPosition.x << std::endl;
//    std::cout << "Y:" << event->globalPosition.y << std::endl;
    switch (event->type) {
        case po::TOUCH_BEGAN_INSIDE_EVENT:
            ((po::RectShape*)event->source->getChild("Rect"))->fillColor.set255(255, 0, 0);
            break;
            
        case po::TOUCH_ENDED_EVENT:
            ((po::RectShape*)event->source->getChild("Rect"))->fillColor.set255(255, 255, 255);
            break;
    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RetinaGraphicsApp::messageHandler(const std::string &msg, const po::Dictionary& dict) {
	
}
