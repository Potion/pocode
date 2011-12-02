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
	
    
	// A. Children hierarchy ///////////////////////
	
	poObject* A = new poObject();
	A->position.set(100, 250, 0);
	addChild(A);
	
	float offsetX = 0;
	for(int i=0; i < 10; i++) {
		
		poOvalShape* circle = new poOvalShape(40, 40, 30);
		circle->fillColor.set(0.8, 0.1, 0.1);
		circle->generateStroke(1);
		circle->strokeColor = poColor::black;
		circle->position.set(offsetX, 0, 0);
		A->addChild(circle);
		
		offsetX += 25;
	}
	
	
	// B. Parents affect children ///////////////////////
	
	poObject* B = new poObject();
	B->position.set(440, 190, 0);
	B->positionTween.set(poPoint(610, 190, 0));
	B->positionTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	B->positionTween.setDuration(3);
	B->positionTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG);
	B->positionTween.start();
	B->scaleTween.set(poPoint(0.6, 0.6, 1));
	B->scaleTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	B->scaleTween.setDuration(1.5);
	B->scaleTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG);
	B->scaleTween.start();
	addChild(B);
	
	poRectShape* head = new poRectShape(120,120);
	head->fillColor.set(0.8, 0.1, 0.1);
	head->generateStroke(1);
	head->strokeColor = poColor::black;
	B->addChild(head);
	
	poRectShape* leftEye = new poRectShape(30,20);
	leftEye->fillColor.set(0.9, 0.5, 0.5);
	leftEye->generateStroke(1);
	leftEye->strokeColor = poColor::black;
	leftEye->position.set(20, 20, 0);
	B->addChild(leftEye);
	
	poRectShape* rightEye = new poRectShape(30,20);
	rightEye->fillColor.set(0.9, 0.5, 0.5);
	rightEye->generateStroke(1);
	rightEye->strokeColor = poColor::black;
	rightEye->position.set(70, 20, 0);
	B->addChild(rightEye);
	
	poRectShape* mouth = new poRectShape(60,20);
	mouth->fillColor.set(0.9, 0.5, 0.5);
	mouth->generateStroke(1);
	mouth->strokeColor = poColor::black;
	mouth->position.set(30, 80, 0);
	B->addChild(mouth);
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
