//
//  Linear Image Example
//  poFramework4
//
//  Created by Miguel Bermudez on 6/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//  
//  Image (c)  Michael Cina 2010 for Ghostly International 
//  http://ghostly.com/wallpapers/cina_verse_2
//

#include "LinearImageExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"



poObject *createObjectForID(uint uid) {
	return new LinearImageExampleApp();
}

void setupApplication() {
	setCurrentPath("../../resources/");
    applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "LinearImageExample", 100, 100, 600, 300);
    
}

void cleanupApplication() {
}

LinearImageExampleApp::LinearImageExampleApp() {
	//load event listeners
    addModifier(new poCamera2D(poColor::black));
    addEvent(PO_MOUSE_MOVE_EVENT, this);
    addEvent(PO_MOUSE_DOWN_EVENT, this);
    addEvent(PO_MOUSE_UP_EVENT, this);
    addEvent(PO_KEY_DOWN_EVENT, this);
    addEvent(PO_KEY_UP_EVENT, this);
    
    //load image
    myImage = new poImage("images/GH_0002d.jpg");
    imageRect  = new poRectShape(myImage);
    imageRect->position(0,0);
       
    //init variables
    mSignal = 0.0f;
    mDirection = 1;
    bMousePressed = false;
    bKeyPressed = false;
    
    //init inidicator line
    mLine = new poShape2D();
    mLine->addPoint(0, 0);
    mLine->addPoint(myImage->width(), 0);
    mLine->strokeColor(poColor::white);
    mLine->generateStroke(2);
    //addChild(mLine);
    
    //init colored lines
    for(int i=0; i<myImage->width(); i++) {
        poLineShape* p = new poLineShape(poPoint(0,0), poPoint(0,getWindowHeight()));
        p->position(i, 0);
        addChild(p);
    }
}

void LinearImageExampleApp::draw() {

    //loop through line of colors vector and set fill for each line
    for (int i=0; i<pixelWideLineColors.size(); i++) {
        poLineShape* p = getChildAs<poLineShape>(this, i);
        p->strokeColor(pixelWideLineColors[i]);
    }
}

void LinearImageExampleApp::update() {
    //change direction of indicator line at image height bound
    if (mSignal > myImage->height()-1 || mSignal < 0) {
        mDirection *= -1;
    }
    
    if(bMousePressed) {
        //set indicator line to mouseY position
        mSignal = abs((int)mouseY % (int)myImage->height());
    } else {
        //increment indicator line postion
        mSignal += (0.3f * mDirection);
    }
    
    //get the pixel wide line of colors based on where the indicator line is now
    getPixelWideLine(mSignal);
    
    //move indicator line
    mLine->position(0, mSignal); 
}

void LinearImageExampleApp::getPixelWideLine(int yPos) {
    //clear vector containing line of colors
    pixelWideLineColors.clear();
    
    //populate vector with new line of colors 
    for (int i=0; i<myImage->width(); i++) {
        pixelWideLineColors.push_back(myImage->getPixel(poPoint(i, mSignal)));
        //cout << "#" << i << ": " << myImage->getPixel(poPoint(i, mSignal)) << endl;
    }
}

LinearImageExampleApp::~LinearImageExampleApp() {
    
}

void LinearImageExampleApp::eventHandler(poEvent *event) {
    poPoint eventPos = event->position;
    
    switch(event->type) {
        case PO_MOUSE_MOVE_EVENT:
            mouseX = eventPos.x;
            mouseY = eventPos.y;
            break;
        case PO_MOUSE_DOWN_EVENT:
            bMousePressed = true;
            break;
        case PO_MOUSE_UP_EVENT:
            bMousePressed = false;
            break;
        case PO_KEY_DOWN_EVENT:
            //this if statement prevents the event being fired continuously if a key is remains pressed
            if(!bKeyPressed) {
                addChild(imageRect);
                addChild(mLine);
                cout << "KEY DOWN" << endl;
            }
            bKeyPressed = true;
            break;
        case PO_KEY_UP_EVENT:
            if(bKeyPressed){
                removeChild(imageRect);
                removeChild(mLine);
                cout << "\tKEY UP" << endl;
            }
            bKeyPressed = false;
            break;
    }
}

void LinearImageExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
