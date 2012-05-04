/////////////////////////////////////////
//
// pocode : Alignment
//
/////////////////////////////////////////

#include "AlignmentApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"						// Include poImageShape.h to be able to use poShapes
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes


// APP CONSTRUCTOR. Create all objects here.
AlignmentApp::AlignmentApp() {
	
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
	
													// Define a grid to draw the rectangles
	float col = 250;								// Define columns width
	float row = 135;								// Define rows height
	float x = 150;									// Define initial x
	float y = 200;									// Define initial y
	
	
	// A. PO_ALIGN_TOP_LEFT /////////////////////// 
	
	poRectShape* A;									// Draw a rectangle
	A = new poRectShape(100, 50);
    A->fillColor.set(0.5, 0.7, 0.2);
	A->position.set(x, y, 0);
	A->drawBounds = true;
	addChild( A );
	
	A->setAlignment(PO_ALIGN_TOP_LEFT);				// Set the alignment to top-left
	
	
	// B. PO_ALIGN_TOP_CENTER ///////////////////////
	
	poRectShape* B;									// Draw a rectangle
	B = new poRectShape(100, 50);
    B->fillColor.set(0.5, 0.7, 0.2);
	B->position.set(x + col, y, 0);
	B->drawBounds = true; 
	addChild( B );
    
	B->setAlignment(PO_ALIGN_TOP_CENTER);			// Set the alignment to top-center
	
	
	// C. PO_ALIGN_TOP_RIGHT ///////////////////////
	
	poRectShape* C;									// Draw a rectangle
	C = new poRectShape(100, 50);
    C->fillColor.set(0.5, 0.7, 0.2);
	C->position.set(x + col*2, y, 0);
	C->drawBounds = true;
	addChild( C );
	
	C->setAlignment(PO_ALIGN_TOP_RIGHT);			// Set the alignment to top-right
	
	
	// D. PO_ALIGN_CENTER_LEFT ///////////////////////
	
	poRectShape* D;									// Draw a rectangle
	D = new poRectShape(100, 50);
    D->fillColor.set(0.5, 0.7, 0.2);
	D->position.set(x, y + row, 0);
	D->drawBounds = true;
	addChild( D );
	
	D->setAlignment(PO_ALIGN_CENTER_LEFT);			// Set the alignment to center-left
	
	
	// E. PO_ALIGN_CENTER_CENTER ///////////////////////
	
	poRectShape* E;									// Draw a rectangle
	E = new poRectShape(100, 50);
    E->fillColor.set(0.5, 0.7, 0.2);
	E->position.set(x + col, y + row, 0);
	E->drawBounds = true;
	addChild( E );
	
	E->setAlignment(PO_ALIGN_CENTER_CENTER);		// Set the alignment to center-center
	
	
	// F. PO_ALIGN_CENTER_RIGHT ///////////////////////
	
	poRectShape* F;									// Draw a rectangle
	F = new poRectShape(100, 50);
    F->fillColor.set(0.5, 0.7, 0.2);
	F->position.set(x + col*2, y + row, 0);
	F->drawBounds = true;
	addChild( F );
	
	F->setAlignment(PO_ALIGN_CENTER_RIGHT);			// Set the alignment to center-right
	
	
	// G. PO_ALIGN_BOTTOM_LEFT ///////////////////////
	
	poRectShape* G;									// Draw a rectangle
	G = new poRectShape(100, 50);
    G->fillColor.set(0.5, 0.7, 0.2);
	G->position.set(x, y + row*2, 0);
	G->drawBounds = true;
	addChild( G );
	
	G->setAlignment(PO_ALIGN_BOTTOM_LEFT);			// Set the alignment to bottom-left
	
	
	// H. PO_ALIGN_BOTTOM_CENTER ///////////////////////
	
	poRectShape* H;									// Draw a rectangle
	H = new poRectShape(100, 50);
    H->fillColor.set(0.5, 0.7, 0.2);
	H->position.set(x + col, y + row*2, 0);
	H->drawBounds = true;
	addChild( H );
	
	H->setAlignment(PO_ALIGN_BOTTOM_CENTER);		// Set the alignment to bottom-center
	
	
	// I. PO_ALIGN_BOTTOM_RIGHT ///////////////////////
	
	poRectShape* I;									// Draw a rectangle
	I = new poRectShape(100, 50);
    I->fillColor.set(0.5, 0.7, 0.2);
	I->position.set(x + col*2, y + row*2, 0);
	I->drawBounds = true;
	addChild( I );
	
	I->setAlignment(PO_ALIGN_BOTTOM_RIGHT);			// Set the alignment to bottom-right
}


// APP DESTRUCTOR. Delete all objects here.
AlignmentApp::~AlignmentApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void AlignmentApp::update() {	
}

// DRAW. Called once per frame. Draw objects here.
void AlignmentApp::draw() {	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void AlignmentApp::eventHandler(poEvent *event) {	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void AlignmentApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
