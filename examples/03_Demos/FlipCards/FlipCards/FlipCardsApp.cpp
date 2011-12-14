#include "FlipCardsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new FlipCardsApp();
}

void setupApplication() {
    // set resources path
	lookUpAndSetPath("resources");
    
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "FlipCards", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

FlipCardsApp::FlipCardsApp() {
    poPoint camera_position	= poPoint(-getWindowWidth()/2,-getWindowHeight()/2,-getWindowWidth()*3/4.f);
	poPoint look_position	= poPoint(-getWindowWidth()/2,-getWindowHeight()/2,0);
	
	poPerspectiveCamera* Cam = new poPerspectiveCamera(45, .001, 1000);
	Cam->cameraPosition( camera_position );
	Cam->lookAtPosition( look_position );
	addModifier(Cam);
	
    // add a matrix of flip cards here
    int numRows = 5;
    int numCols = 5;
    
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            card = new FlipCard();
            card->position.set(i*getWindowWidth()/numRows, j*getWindowHeight()/numCols, 0);
            addChild(card);
        }
    }
}

FlipCardsApp::~FlipCardsApp() {
}

void FlipCardsApp::eventHandler(poEvent *event) {
	
}

void FlipCardsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
