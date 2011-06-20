//
//  Load Image Example
//  poFramework4
//
//  Created by Miguel Bermudez on 6/17/11.
//  Copyright 2011 Potion Design. All rights reserved.
//  
//  Image (c)  Michael Cina 2010 for Ghostly International 
//  http://ghostly.com/wallpapers/cina_verse_2
//

#include "testApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new testApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "test", 100, 100, 600, 300);
    
}

void cleanupApplication() {
}

testApp::testApp() {
	addModifier(new poCamera2D(poColor::black));
    
    //load image
    myImage = new poImage("../../resources/images/GH_0002d.jpg");
    //create new poRectShape and give it an image to create a texture quickly
    image = new poRectShape(myImage);
    //set alignment to center for horiz and vert
    image->alignment(PO_ALIGN_CENTER_CENTER);
    //position image in the center of the screen
    image->position(getWindowWidth()/2, getWindowHeight()/2);
    //add poRect (image) to stage
    addChild(image);
}

void testApp::draw() {
}

void testApp::update() {
}


testApp::~testApp() {
}

void testApp::eventHandler(poEvent *event) {
	
}

void testApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
