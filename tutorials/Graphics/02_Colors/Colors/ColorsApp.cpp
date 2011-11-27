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
	
	poRectShape* rect = new poRectShape(135, 75);
	rect->fillColor = A;
	rect->position.set(57, 232, 0);
	addChild(rect);
	
	
	// B. RGB (0 to 1) ///////////////////////
	
	poColor B;
	B.set(1, 0.75, 0, 1);
	
	rect = new poRectShape(135, 75);
	rect->fillColor = B;
	rect->position.set(240, 232, 0);
	addChild(rect);
	
	
	// C. RGB (0 to 255) ///////////////////////
	
	poColor C;
	C.set255(255, 255, 0, 255);
	
	rect = new poRectShape(135, 75);
	rect->fillColor = C;
	rect->position.set(423, 232, 0);
	addChild(rect);
	
	
	// D. HSV ///////////////////////
	
	poColor D;
	D.setHSV(0.45, 1.0, 1.0);
	
	rect = new poRectShape(135, 75);
	rect->fillColor = D;
	rect->position.set(607, 232, 0);
	addChild(rect);
	
	
	// E. Set with a string ///////////////////////
	
	poColor E;
	E.set("#00CCFF");
	
	rect = new poRectShape(135, 75);
	rect->fillColor = E;
	rect->position.set(57, 403, 0);
	addChild(rect);
	
	
	// F. Set individual values ///////////////////////
	
	poColor F = poColor(0, 0, 0);
	F.R = 0;
	F.G = 0;
	F.B = 1;
	F.A = 1;
	
	rect = new poRectShape(135, 75);
	rect->fillColor = F;
	rect->position.set(240, 403, 0);
	addChild(rect);
	
	
	// G. Blend 2 colors ///////////////////////
	
	poColor G;
	G = blendColors(poColor::blue, poColor::magenta, 0.5); 
	
	rect = new poRectShape(135, 75);
	rect->fillColor = G;
	rect->position.set(423, 403, 0);
	addChild(rect);
	
	
	// H. Random color ///////////////////////
	
	poColor H;
	H = poColor::random();							// INTERNAL: it's not working!!
	
	rect = new poRectShape(135, 75);
	rect->fillColor = H;
	rect->position.set(607, 403, 0);
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
