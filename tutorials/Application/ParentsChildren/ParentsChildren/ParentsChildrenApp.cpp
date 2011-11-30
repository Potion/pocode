/////////////////////////////////////////
//
// LESSON : Parents and children
//
/////////////////////////////////////////

#include "ParentsChildrenApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"


// APP CONSTRUCTOR.
// Create all objects here.
ParentsChildrenApp::ParentsChildrenApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
    
	// A. Children positions relative to parents ///////////////////////
	
	poObject* parent = new poObject();
	parent->position.set(150, 300, 0);
	addChild(parent);
	
	poOvalShape* circle = new poOvalShape(40, 40, 30);
    circle->fillColor.set(0.8, 0.2, 0.2);
    circle->position.set(-30, 0, 0);
    parent->addChild(circle);
    
    poOvalShape* circleB = new poOvalShape(40, 40, 30);
    circleB->fillColor.set(0.8, 0.2, 0.2);
    circleB->position.set(30, 0, 0);
    parent->addChild(circleB);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
ParentsChildrenApp::~ParentsChildrenApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void ParentsChildrenApp::update() {
}

// EVENT HANDLER
// Respond to user events here.
void ParentsChildrenApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void ParentsChildrenApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
