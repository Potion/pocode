/////////////////////////////////////////
//
// pocode : Points
//
/////////////////////////////////////////

#include "PointsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes
#include "poShapeBasics2D.h"						// Include poImageShape.h to be able to use poShapes
#include "poTextBox.h"								// include poTextBox.h to be able to use poTextBox


// APP CONSTRUCTOR. Create all objects here.
PointsApp::PointsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Common poPoints: position and scale ///////////////////////
	
	poPoint A = poPoint(50, 50);						// Define poPoint A
	poPoint scaleA = poPoint(2, 2, 1);					// Define poPoint scaleA
	
	poObject* holderA = new poObject();					// Offset everything to the white rectangle
	holderA->position.set(50, 166, 0);
	addChild(holderA);
	
	poOvalShape* oval = new poOvalShape(5,5,10);		// Draw pointA as an oval
	oval->fillColor = poColor::black;
	oval->position = A;									// Set the position equal to poPoint A
	oval->scale = scaleA;								// Set the scale equal to poPoint scaleA
	holderA->addChild(oval);							// Add the circle to holderA
	
	
	// B. poPoints: + and - ///////////////////////
	
	poPoint B = poPoint(50, 25);								// Define poPoint B
	poPoint diff = A - B;										// Define a poPoint as A - B
	poPoint sum = A + B;										// Define a poPoint as A + B
	
	poObject* holderB = new poObject();							// Offset everything to the white rectangle
	holderB->position.set(295, 166, 0);
	addChild(holderB);
	
	poOvalShape* ovalA = new poOvalShape(5, 5, 10);				// Draw A as a black circle
	ovalA->fillColor = poColor::black;
	ovalA->position = A;										// Set the position equal to poPoint A
	holderB->addChild(ovalA);									// Add the circle to holderB
	
	poOvalShape* ovalB = new poOvalShape(5, 5, 10);				// Draw B as a black circle
	ovalB->fillColor = poColor::black;
	ovalB->position = B;										// Set the position equal to poPoint B
	holderB->addChild(ovalB);									// Add the circle to holderB
	
	poLineShape* diffLine = new poLineShape(poPoint(0,0), diff); // Draw diff as a BLUE line
																// from 0,0 to poPoint diff
	diffLine->generateStroke(2);
	diffLine->strokeColor = poColor::blue;
	holderB->addChild(diffLine);								// Add the line to holderB
	
	// Draw sum as a line
	poLineShape* sumLine = new poLineShape(poPoint(0,0), sum);	// Draw sum as a RED line
																// from 0,0 to poPoint sum
	sumLine->generateStroke(2);
	sumLine->strokeColor = poColor::red;
	holderB->addChild(sumLine);									// Add the line to holderB
	
	
	// C. poPoints: * and / ///////////////////////
	
	poPoint C = poPoint(40, 60, 0);								// Define poPoint C
	poPoint mult = C * 2;										// Define a poPoint as C * 2
	poPoint div = C / 2;										// Define a poPoint as C / 2
	
	poObject* holderC = new poObject();							// Offset everything to the white rectangle
	holderC->position.set(538, 166, 0);
	addChild(holderC);
	
	poOvalShape* ovalC = new poOvalShape(5, 5, 10);				// Draw C as a black circle
	ovalC->fillColor = poColor::black;
	ovalC->position = C;										// Set the position equal to poPoint C
	holderC->addChild(ovalC);									// Add the line to holderC
	
	poLineShape* multLine = new poLineShape(poPoint(0,0), mult); // Draw mult as a RED line
																// from 0,0 to poPoint mult
	multLine->generateStroke(2);
	multLine->strokeColor = poColor::red;
	holderC->addChild(multLine);								// Add the line to holderC
	
	poLineShape* divLine = new poLineShape(poPoint(0,0), div);	// Draw div as a BLUE line
																// from 0,0 to poPoint div
	divLine->generateStroke(2);
	divLine->strokeColor = poColor::blue;
	holderC->addChild(divLine);									// Add the line to holderC
}


// APP DESTRUCTOR. Delete all objects here.
PointsApp::~PointsApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void PointsApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void PointsApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void PointsApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void PointsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
