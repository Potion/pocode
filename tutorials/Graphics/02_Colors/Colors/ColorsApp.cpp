/////////////////////////////////////////
//
// LESSON : Colors
//
/////////////////////////////////////////

#include "ColorsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR.
// Create all objects here.
ColorsApp::ColorsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
ColorsApp::~ColorsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void ColorsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void ColorsApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void ColorsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
