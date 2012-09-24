/*	Created by Steve Varga on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poAsyncImageLoaderExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poImage.h"
#include "poURLLoader.h"


// APP CONSTRUCTOR. Create all objects here.
poAsyncImageLoaderExampleApp::poAsyncImageLoaderExampleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    imageHolder = new poRectShape(50,50);
    imageHolder->setAlignment(PO_ALIGN_CENTER_CENTER);
    imageHolder->fillColor.set(1.0, 1.0, 1.0);
    
    addChild(imageHolder);
    
    poURL url("http://www.imgbase.info/images/safe-wallpapers/photography/miscellaneous/19757_miscellaneous_delirium_beer.jpg");
    //poImage::getImageAsync(url, this, poFilePath("beer.jpg"));
    
    poURLLoader::getFile(url);
    
    //poImage::getImageAsync(poFilePath("beer.jpg"), this);
    
}

// APP DESTRUCTOR. Delete all objects here.
poAsyncImageLoaderExampleApp::~poAsyncImageLoaderExampleApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poAsyncImageLoaderExampleApp::update() {
    imageHolder->rotation++;
	imageHolder->position.set(po::getWindowWidth()/2,
                              po::getWindowHeight()/2,
                              1.0);
}

// DRAW. Called once per frame. Draw objects here.
void poAsyncImageLoaderExampleApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poAsyncImageLoaderExampleApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poAsyncImageLoaderExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	if(msg == poImageLoaderCompleteMessage) {
        if(dict.getString("status") == poImageLoaderSuccessMessage) {
            poImage *img = (poImage*)dict.getPtr("image");
            imageHolder->reshape(img->getWidth(), img->getHeight());
            imageHolder->placeTexture(new poTexture(img));
            delete img;
        } else {
            std::cout << "Fail." << std::endl;
        }
    }
}
