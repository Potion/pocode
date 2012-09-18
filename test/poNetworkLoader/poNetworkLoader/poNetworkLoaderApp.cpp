/*	Created by Steve Varga on 8/23/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poNetworkLoaderApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poURLLoader.h"

#include "poThreadCenter.h"


// APP CONSTRUCTOR. Create all objects here.
poNetworkLoaderApp::poNetworkLoaderApp() {
	addModifier(new poCamera2D(poColor::black));
    
    poURLLoader::getFileAsync(poURL("http://www.vargatron.com/"), this, poFilePath("site.html"));
    //poURLLoader::getFileAsStringAsync(poURL("http://www.vargatron.com"), this);
    
    p = new poOvalShape(50, 50, 100);
    addChild(p);
    
    p->position.set(getWindowWidth()/2, getWindowHeight()/2, 1.0f);
    

}

// APP DESTRUCTOR. Delete all objects here.
poNetworkLoaderApp::~poNetworkLoaderApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void poNetworkLoaderApp::update() {
    
    float newScale = 1 + sin(poGetElapsedTime());
    p->scale.set(newScale, newScale, newScale);
}

// DRAW. Called once per frame. Draw objects here.
void poNetworkLoaderApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void poNetworkLoaderApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void poNetworkLoaderApp::messageHandler(const std::string &msg, const poDictionary& dict) {
    if(msg == poURLLoaderCompleteMessage) {
        switch (dict.getInt("mode")) {
            case PO_FILE_LOADER_MODE_SAVE:
                std::cout << "Saved file as " << dict.getString("filePath") << std::endl;
                break;
                
            case PO_FILE_LOADER_MODE_RETURN_AS_STRING:
                std::cout << dict.getString("content") << std::endl;
                break;
        }
    }
}
