/////////////////////////////////////////
//
// LESSON : Basic Tweens
//
/////////////////////////////////////////

#include "BasicTweensApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR.
// Create all objects here.
BasicTweensApp::BasicTweensApp() {
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. Position Tween ///////////////////////
	
	A = new poRectShape(30, 30);
    A->fillColor.set(0.5, 0.8, 0.9);
	A->generateStroke(1);
	A->strokeColor = poColor::black;
	A->position.set(125, 300, 0);
	A->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( A );
	
	A->positionTween.set(poPoint(125, 245, 0));					// Set the final position
	A->positionTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);	// Set the type of easing function
	A->positionTween.setDuration(2.0);							// Set the duration in seconds
	A->positionTween.start();									// Start the animation
	
	
	// B. Scale Tween ///////////////////////
	
	B = new poRectShape(30, 30);
    B->fillColor.set(0.5, 0.8, 0.9);
	B->generateStroke(1);
	B->strokeColor = poColor::black;
	B->position.set(307, 300, 0);
	B->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( B );
	
	B->scaleTween.set(poPoint(2, 2, 1));						// Set the final scale value (x, y, z)
	B->scaleTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	B->scaleTween.setDuration(2.0);
	B->scaleTween.start();
	
	
	// C. Rotation Tween ///////////////////////
	
	C = new poRectShape(30, 30);
    C->fillColor.set(0.5, 0.8, 0.9);
	C->generateStroke(1);
	C->strokeColor = poColor::black;
	C->position.set(490, 300, 0);
	C->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( C );
	
	C->rotationTween.set(225);									// Set the rotation in degrees
	C->rotationTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	C->rotationTween.setDuration(2.0);
	C->rotationTween.setDelay(1.0);								// Set the delay, it waits 1 second before it starts
	C->rotationTween.start();
	
	
    // D. Alpha Tween ///////////////////////
	
	D = new poRectShape(30, 30);
    D->fillColor.set(0.5, 0.8, 0.9);
	D->generateStroke(1);
	D->strokeColor = poColor::black;
	D->position.set(673, 300, 0);
	D->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( D );
	
	D->alphaTween.set(0);
	D->alphaTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	D->alphaTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG, 1);		// Make it repeat, 1 time
																// PO_TWEEN_REPEAT_PINGPONG will make alpha go back to its original state
	D->alphaTween.setDuration(2.0);
	D->alphaTween.start();
	
	
	// key to restart the tweens
	addEvent(PO_KEY_DOWN_EVENT, this);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
BasicTweensApp::~BasicTweensApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void BasicTweensApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void BasicTweensApp::eventHandler(poEvent *event) {
	
	if(event->keyChar == 'r') {
		A->stopAllTweens();
		A->position.set(125, 300, 0);
		A->positionTween.start();
		
		B->stopAllTweens();
		B->scale.set(1, 1, 1);
		B->scaleTween.start();
		
		C->stopAllTweens();
		C->rotation = 0;
		C->rotationTween.start();
		
		D->stopAllTweens();
		D->alpha = 1.f;
		D->alphaTween.set(0);
		D->alphaTween.start();
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void BasicTweensApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
