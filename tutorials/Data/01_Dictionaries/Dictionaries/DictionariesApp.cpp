/////////////////////////////////////////
//
// LESSON : Dictionaries
//
/////////////////////////////////////////

#include "DictionariesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"


// APP CONSTRUCTOR.
// Create all objects here.
DictionariesApp::DictionariesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Create a poDictionary ///////////////////////
	
	
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
DictionariesApp::~DictionariesApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void DictionariesApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void DictionariesApp::eventHandler(poEvent *event) {
	
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void DictionariesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
