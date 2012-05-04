/////////////////////////////////////////
//
// pocode : Basic Tweens
//
/////////////////////////////////////////

#include "BasicTweensApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"


// APP CONSTRUCTOR. Create all objects here.
BasicTweensApp::BasicTweensApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Position Tween ///////////////////////
	
	A = new poRectShape(30, 30);								// Draw a rectangle that will change position
    A->fillColor.set(0.5, 0.8, 0.9);
	A->generateStroke(1);
	A->strokeColor = poColor::black;
	A->position.set(125, 240, 0);
	A->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( A );
	
	A->positionTween.set(poPoint(125, 195, 0));					// Set the final position
	A->positionTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);	// Set the type of easing function
	A->positionTween.setDuration(2.0);							// Set the duration in seconds
	A->positionTween.start();									// Start the animation
	
	
	// B. Scale Tween ///////////////////////
	
	B = new poRectShape(30, 30);								// Draw a rectangle that will change scale
    B->fillColor.set(0.5, 0.8, 0.9);
	B->generateStroke(1);
	B->strokeColor = poColor::black;
	B->position.set(307, 240, 0);
	B->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( B );
	
	B->scaleTween.set(poPoint(2, 2, 1));						// Set the final scale value (x, y, z)
	B->scaleTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);		// Set the type of easing function
	B->scaleTween.setDuration(2.0);								// Set the duration in seconds
	B->scaleTween.start();										// Start the animation
	
	
	// C. Rotation Tween ///////////////////////
	
	C = new poRectShape(30, 30);								// Draw a rectangle that will rotate
    C->fillColor.set(0.5, 0.8, 0.9);
	C->generateStroke(1);
	C->strokeColor = poColor::black;
	C->position.set(490, 240, 0);
	C->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( C );
	
	C->rotationTween.set(360);									// Set the rotation in degrees
	C->rotationTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);	// Set the type of easing function
	C->rotationTween.setDuration(2.0);							// Set the duration in seconds
	C->rotationTween.setRepeat(PO_TWEEN_REPEAT_REGULAR);		// Make it repeat when it's done
	C->rotationTween.setDelay(1.0);								// Set the delay, it waits 1 second before it starts
	C->rotationTween.start();									// Start the animation
	
	
    // D. Alpha Tween ///////////////////////
	
	D = new poRectShape(30, 30);								// Draw a rectangle that will change alpha
    D->fillColor.set(0.5, 0.8, 0.9);
	D->generateStroke(1);
	D->strokeColor = poColor::black;
	D->position.set(673, 240, 0);
	D->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild( D );
	
	D->alphaTween.set(0);										// Set the final alpha value (0 to 1)
	D->alphaTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);		// Set the type of easing function
	D->alphaTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG, 1);		// Make it repeat, 1 time
																// PO_TWEEN_REPEAT_PINGPONG will make alpha go back to its original state
	D->alphaTween.setDuration(2.0);								// Set the duration in seconds
	D->alphaTween.start();										// Start the animation
	
	
	// key to restart the tweens
	addEvent(PO_KEY_DOWN_EVENT, this);
}

// APP DESTRUCTOR. Delete all objects here.
BasicTweensApp::~BasicTweensApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void BasicTweensApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void BasicTweensApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void BasicTweensApp::eventHandler(poEvent *event) {
	
	if(event->keyChar == 'r') {									// If you press 'r'
		A->stopAllTweens();										// Stop all the tweens in rectangle A
		A->position.set(125, 240, 0);							// Reset its position to the original position
		A->positionTween.start();								// Start the position tween again
		
		B->stopAllTweens();										// Stop all the tweens in rectangle B
		B->scale.set(1, 1, 1);									// Reset its scale to the original scale
		B->scaleTween.start();									// Start the scale tween again
		
		C->stopAllTweens();										// Stop all the tweens in rectangle C
		C->rotation = 0;										// Reset its rotation to the original rotation
		C->rotationTween.start();								// Start the rotation tween again
		
		D->stopAllTweens();										// Stop all the tweens in rectangle D
		D->alpha = 1.f;											// Reset its alpha to the original alpha
		D->alphaTween.set(0);									// Since the tween is repeating ping pong style,
																// we need to reset also the final value
		D->alphaTween.start();									// Start the alpha tween again
	}
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void BasicTweensApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
