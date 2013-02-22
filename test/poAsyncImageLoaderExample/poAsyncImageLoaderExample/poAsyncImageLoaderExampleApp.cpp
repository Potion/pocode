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
	addModifier(new po::Camera2D(po::Color::black));
    
    imageHolder = new po::RectShape(50,50);
    imageHolder->setAlignment(po::PO_ALIGN_CENTER_CENTER);
    imageHolder->fillColor.set(1.0, 1.0, 1.0);
    
    addChild(imageHolder);
    
    po::URL url("http://theislanderinseattle.com/wp-content/uploads/2012/04/Seattle_West_View_Space_Needle_5_Frommer_contest.jpg");
    po::Image::getImageAsync(url, this);
    
    po::URLLoader::getFile(url);
    
    //po::Image::getImageAsync(po::FilePath("beer.jpg"), this);
    
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
void poAsyncImageLoaderExampleApp::eventHandler(po::Event *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poAsyncImageLoaderExampleApp::messageHandler(const std::string &msg, const po::Dictionary& dict) {
	if(msg == po::ImageLoaderCompleteMessage) {
        if(dict.getString("status") == po::ImageLoaderSuccessMessage) {
            po::Image *img = (po::Image*)dict.getPtr("image");
            imageHolder->reshape(img->getWidth(), img->getHeight());
            imageHolder->placeTexture(new po::Texture(img));
            delete img;
        } else {
            std::cout << "Fail." << std::endl;
        }
    }
}
