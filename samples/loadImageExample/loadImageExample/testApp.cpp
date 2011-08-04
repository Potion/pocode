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

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

poObject *createObjectForID(uint uid) {
	return new testApp();
}

void setupApplication() {
	// set the pwd to a directory above the app named resources
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "test", 100, 100, 600, 300);
}

void cleanupApplication() {
}

testApp::testApp() {
	addModifier(new poCamera2D());
    
    //load image from pwd
    myImage = getImage("images/GH_0002d.jpg");
//	applicationReshapeWindow(applicationCurrentWindow(), poRect(0,0,myImage->width(),myImage->height()));
	
    //create new poRectShape and give it an image to create a texture quickly
    image = new poRectShape(myImage->width(), myImage->height());
	image->placeTexture( myImage->texture() );
    //set alignment to center for horiz and vert
    image->alignment(PO_ALIGN_CENTER_CENTER);
    //position image in the center of the screen
    image->position.set(getWindowWidth()/2, getWindowHeight()/2, 0);
    //add poRect (image) to stage
    addChild(image);
}

testApp::~testApp() {}
