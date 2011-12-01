/////////////////////////////////////////
//
// LESSON : Alignment
//
/////////////////////////////////////////

#include "AlignmentApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"


// APP CONSTRUCTOR.
// Create all objects here.
AlignmentApp::AlignmentApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	float offsetX = 250;
	float offsetY = 135;
	float x = 150;
	float y = 200;
	
	
	// A. PO_ALIGN_TOP_LEFT /////////////////////// 
	
	poRectShape* A; 
	A = new poRectShape(100, 50);
    A->fillColor.set(0.5, 0.7, 0.2);
	A->position.set(x, y, 0);
	A->drawBounds = true;
	addChild( A );
	
	A->setAlignment(PO_ALIGN_TOP_LEFT);
	
	
	// B. PO_ALIGN_TOP_CENTER ///////////////////////
	
	poRectShape* B;
	B = new poRectShape(100, 50);
    B->fillColor.set(0.5, 0.7, 0.2);
	B->position.set(x + offsetX, y, 0);
	B->drawBounds = true; 
	addChild( B );
    
	B->setAlignment(PO_ALIGN_TOP_CENTER);
	
	
	// C. PO_ALIGN_TOP_RIGHT ///////////////////////
	
	poRectShape* C;
	C = new poRectShape(100, 50);
    C->fillColor.set(0.5, 0.7, 0.2);
	C->position.set(x + offsetX*2, y, 0);
	C->drawBounds = true;
	addChild( C );
	
	C->setAlignment(PO_ALIGN_TOP_RIGHT);
	
	
	// D. PO_ALIGN_CENTER_LEFT ///////////////////////
	
	poRectShape* D;
	D = new poRectShape(100, 50);
    D->fillColor.set(0.5, 0.7, 0.2);
	D->position.set(x, y + offsetY, 0);
	D->drawBounds = true;
	addChild( D );
	
	D->setAlignment(PO_ALIGN_CENTER_LEFT);
	
	
	// E. PO_ALIGN_CENTER_CENTER ///////////////////////
	
	poRectShape* E;
	E = new poRectShape(100, 50);
    E->fillColor.set(0.5, 0.7, 0.2);
	E->position.set(x + offsetX, y + offsetY, 0);
	E->drawBounds = true;
	addChild( E );
	
	E->setAlignment(PO_ALIGN_CENTER_CENTER);
	
	
	// F. PO_ALIGN_CENTER_RIGHT ///////////////////////
	
	poRectShape* F;
	F = new poRectShape(100, 50);
    F->fillColor.set(0.5, 0.7, 0.2);
	F->position.set(x + offsetX*2, y + offsetY, 0);
	F->drawBounds = true;
	addChild( F );
	
	F->setAlignment(PO_ALIGN_CENTER_RIGHT);
	
	
	// G. PO_ALIGN_BOTTOM_LEFT ///////////////////////
	
	poRectShape* G;
	G = new poRectShape(100, 50);
    G->fillColor.set(0.5, 0.7, 0.2);
	G->position.set(x, y + offsetY*2, 0);
	G->drawBounds = true;
	addChild( G );
	
	G->setAlignment(PO_ALIGN_BOTTOM_LEFT);
	
	
	// H. PO_ALIGN_BOTTOM_CENTER ///////////////////////
	
	poRectShape* H;
	H = new poRectShape(100, 50);
    H->fillColor.set(0.5, 0.7, 0.2);
	H->position.set(x + offsetX, y + offsetY*2, 0);
	H->drawBounds = true;
	addChild( H );
	
	H->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	
	
	// I. PO_ALIGN_BOTTOM_RIGHT ///////////////////////
	
	poRectShape* I;
	I = new poRectShape(100, 50);
    I->fillColor.set(0.5, 0.7, 0.2);
	I->position.set(x + offsetX*2, y + offsetY*2, 0);
	I->drawBounds = true;
	addChild( I );
	
	I->setAlignment(PO_ALIGN_BOTTOM_RIGHT);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
AlignmentApp::~AlignmentApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void AlignmentApp::update() {	
}


// EVENT HANDLER
// Respond to user events here.
void AlignmentApp::eventHandler(poEvent *event) {	
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AlignmentApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
