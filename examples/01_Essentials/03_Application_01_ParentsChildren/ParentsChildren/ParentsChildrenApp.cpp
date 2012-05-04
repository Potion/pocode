/////////////////////////////////////////
//
// pocode : Parents and children
//
/////////////////////////////////////////

#include "ParentsChildrenApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"								// include poTextBox.h to be able to use poShape
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes


// APP CONSTRUCTOR. Create all objects here.
ParentsChildrenApp::ParentsChildrenApp() {
	
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
	
    
	// A. Children hierarchy ///////////////////////
	
	poObject* A = new poObject();
	A->position.set(200, 250, 0);
	addChild(A);
	
	poOvalShape* firstCircle;
	firstCircle = new poOvalShape(40, 40, 30);			// Draw a circle, the first child of A
	firstCircle->fillColor.set(0.8, 0.1, 0.1);
	firstCircle->generateStroke(1);
	firstCircle->strokeColor = poColor::black;
	firstCircle->position.set(0, 0, 0);					// The position is relative to the parent's position
														// So even if it's 0, the circle will appear
														// in the same position as A
	A->addChild(firstCircle);							// Add the circle as a child of A
	
	
	poOvalShape* secondCircle;
	secondCircle = new poOvalShape(40, 40, 30);			// Draw another circle, the second child of A
	secondCircle->fillColor.set(0.8, 0.1, 0.1);
	secondCircle->generateStroke(1);
	secondCircle->strokeColor = poColor::black;
	secondCircle->position.set(25, 0, 0);				// Set the position a little bit more on the right
	A->addChild(secondCircle);							// Add the circle as a child of A
	
	
	// B. Parents affect children ///////////////////////
	
	poObject* B = new poObject();							// Create a poObject to hold together some shapes
	B->position.set(440, 190, 0);							// Make B update its position with a positionTween
	B->positionTween.set(poPoint(610, 190, 0));
	B->positionTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	B->positionTween.setDuration(3);
	B->positionTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG);
	B->positionTween.start();
	B->scaleTween.set(poPoint(0.6, 0.6, 1));				// Make B update its scale with a scaleTween
	B->scaleTween.setTweenFunction(PO_TWEEN_LINEAR_FUNC);
	B->scaleTween.setDuration(1.5);
	B->scaleTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG);
	B->scaleTween.start();
	addChild(B);
	
	poRectShape* head = new poRectShape(120,120);			// Create a rectangle that represents a head
	head->fillColor.set(0.8, 0.1, 0.1);
	head->generateStroke(1);
	head->strokeColor = poColor::black;
	B->addChild(head);										// Add the head to the parent B
	
	poRectShape* leftEye = new poRectShape(30,20);			// Create a rectangle that represents an eye
	leftEye->fillColor.set(0.9, 0.5, 0.5);
	leftEye->generateStroke(1);
	leftEye->strokeColor = poColor::black;
	leftEye->position.set(20, 20, 0);
	B->addChild(leftEye);									// Add the left eye to the parent B
	
	poRectShape* rightEye = new poRectShape(30,20);			// Create a rectangle that represents an eye
	rightEye->fillColor.set(0.9, 0.5, 0.5);
	rightEye->generateStroke(1);
	rightEye->strokeColor = poColor::black;
	rightEye->position.set(70, 20, 0);
	B->addChild(rightEye);									// Add the right eye to the parent B
	
	poRectShape* mouth = new poRectShape(60,20);			// Create a rectangle that represents a mouth
	mouth->fillColor.set(0.9, 0.5, 0.5);
	mouth->generateStroke(1);
	mouth->strokeColor = poColor::black;
	mouth->position.set(30, 80, 0);
	B->addChild(mouth);										// Add the mouth to the parent B
}

// APP DESTRUCTOR. Delete all objects here.
ParentsChildrenApp::~ParentsChildrenApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void ParentsChildrenApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void ParentsChildrenApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void ParentsChildrenApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void ParentsChildrenApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
