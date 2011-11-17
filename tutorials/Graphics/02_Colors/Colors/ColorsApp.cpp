/////////////////////////////////////////
//
// LESSON : Colors
//
/////////////////////////////////////////

#include "ColorsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR.
// Create all objects here.
ColorsApp::ColorsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. RGB (0 to 1) ///////////////////////
	
	poColor A;
	A = poColor::red;
	
	poRectShape* rect = new poRectShape(100, 70);
	rect->fillColor = A;
	rect->position.set(60, 223, 0);
	addChild(rect);
	
	
	// B. RGB (0 to 1) ///////////////////////
	
	poColor B;
	B.set(0, 1, 0, 1);
	
	rect = new poRectShape(100, 70);
	rect->fillColor = B;
	rect->position.set(205, 223, 0);
	addChild(rect);
	
	
	// C. RGB (0 to 255) ///////////////////////
	
	poColor C;
	C.set(0, 0, 255, 255);
	
	rect = new poRectShape(100, 70);
	rect->fillColor = C;
	rect->position.set(350, 223, 0);
	addChild(rect);
	
	
	// D. RGB (0 to 255) ///////////////////////
	
	poColor D;
	D.setHSV(0.5, 1, 1);
	
	rect = new poRectShape(100, 70);
	rect->fillColor = D;
	rect->position.set(495, 223, 0);
	addChild(rect);
	
	
	// E. RGB (0 to 255) ///////////////////////
	
	poColor E;
	E.set(1, 0, 0);
	
	rect = new poRectShape(100, 70);
	rect->fillColor = E;
	rect->position.set(640, 223, 0);
	addChild(rect);
	
	
	// F. RGB (0 to 255) ///////////////////////
	
	poColor F;
	F.set(1, 0, 0);
	
	
	// G. RGB (0 to 255) ///////////////////////
	
	poColor G;
	G.set(1, 0, 0);
	
	
	// H. RGB (0 to 255) ///////////////////////
	
	poColor H;
	H.set(1, 0, 0);
	
	
	// I. RGB (0 to 255) ///////////////////////
	
	poColor I;
	I.set(1, 0, 0);
	
	
	// J. RGB (0 to 255) ///////////////////////
	
	poColor J;
	J = blendColors(poColor::red, poColor::green, .5);
	
	rect = new poRectShape(100, 70);
	rect->fillColor = J;
	rect->position.set(640, 403, 0);
	addChild(rect);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
ColorsApp::~ColorsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void ColorsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void ColorsApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void ColorsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
