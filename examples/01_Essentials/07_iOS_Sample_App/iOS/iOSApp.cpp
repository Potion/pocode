/////////////////////////////////////////
//
// pocode : iOS App
//
/////////////////////////////////////////

#include "iOSApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR. Create all objects here.
iOSApp::iOSApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poOvalShape* circle = new poOvalShape( 50, 50, 50 );
	circle->position.set( getWindowWidth()/2, getWindowHeight()/2, 0);
	circle->fillColor = poColor::magenta;
	addChild( circle );
}

// APP DESTRUCTOR. Delete all objects here.
iOSApp::~iOSApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void iOSApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void iOSApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void iOSApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void iOSApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
