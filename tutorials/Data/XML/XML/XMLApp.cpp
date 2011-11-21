/////////////////////////////////////////
//
// LESSON : XML
//
/////////////////////////////////////////

#include "XMLApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR.
// Create all objects here.
XMLApp::XMLApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. Load XML Data ///////////////////////
	
	
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
XMLApp::~XMLApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void XMLApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void XMLApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void XMLApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
