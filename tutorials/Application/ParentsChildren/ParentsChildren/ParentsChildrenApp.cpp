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
	
	poShape2D* body = new poOvalShape(60, 60, 30);
	body->fillColor.set(0.8, 0.6, 0.8);
	body->position.set(0, 15, 0);
	body->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	parent->addChild(body);
	
	poShape2D* head = new poOvalShape(20, 20, 50);
	head->fillColor.set(0.8, 0.6, 0.8);
	head->position.set(0, 15-60, 0);
	head->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	parent->addChild(head);
	
	poRectShape* rightLeg = new poRectShape(5, 30);
	rightLeg->fillColor.set(0.8, 0.6, 0.8);
	rightLeg->rotation = -25;
	rightLeg->setAlignment(PO_ALIGN_TOP_CENTER);
	rightLeg->offset += poPoint(0, 15, 0);
	parent->addChild(rightLeg);
	
	poRectShape* leftLeg = new poRectShape(5, 30);
	leftLeg->fillColor.set(0.8, 0.6, 0.8);
	leftLeg->rotation = 25;
	leftLeg->setAlignment(PO_ALIGN_TOP_CENTER);
	leftLeg->offset += poPoint(0, 15, 0);
	parent->addChild(leftLeg);
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
