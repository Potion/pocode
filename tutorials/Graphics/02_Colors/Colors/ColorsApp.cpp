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
	
	poRectShape* rectA = new poRectShape(135, 75);
	rectA->fillColor = A;
	rectA->position.set(57, 232, 0);
	addChild(rectA);
	
	
	// B. RGB (0 to 1) ///////////////////////
	
	poColor B;
	B.set(1, 0.75, 0, 1);
	
	poRectShape* rectB = new poRectShape(135, 75);
	rectB->fillColor = B;
	rectB->position.set(240, 232, 0);
	addChild(rectB);
	
	
	// C. RGB (0 to 255) ///////////////////////
	
	poColor C;
	C.set255(255, 255, 0, 255);
	
	poRectShape* rectC = new poRectShape(135, 75);
	rectC->fillColor = C;
	rectC->position.set(423, 232, 0);
	addChild(rectC);
	
	
	// D. HSV ///////////////////////
	
	poColor D;
	D.setHSV(0.45, 1.0, 1.0);
	
	poRectShape* rectD = new poRectShape(135, 75);
	rectD->fillColor = D;
	rectD->position.set(607, 232, 0);
	addChild(rectD);
	
	
	// E. Set with a string ///////////////////////
	
	poColor E;
	E.set("#00CCFF");
	
	poRectShape* rectE = new poRectShape(135, 75);
	rectE->fillColor = E;
	rectE->position.set(57, 403, 0);
	addChild(rectE);
	
	
	// F. Set individual values ///////////////////////
	
	poColor F = poColor(0, 0, 0);
	F.R = 0;
	F.G = 0;
	F.B = 1;
	F.A = 1;
	
	poRectShape* rectF = new poRectShape(135, 75);
	rectF->fillColor = F;
	rectF->position.set(240, 403, 0);
	addChild(rectF);
	
	
	// G. Blend 2 colors ///////////////////////
	
	poColor G;
	G = blendColors(poColor::blue, poColor::magenta, 0.5); 
	
	poRectShape* rectG = new poRectShape(135, 75);
	rectG->fillColor = G;
	rectG->position.set(423, 403, 0);
	addChild(rectG);
	
	
	// H. Random color ///////////////////////
	
	poColor H;
	H = poColor::random();							// INTERNAL: it's not working!!
	
	poRectShape* rectH = new poRectShape(135, 75);
	rectH->fillColor = H;
	rectH->position.set(607, 403, 0);
	addChild(rectH);
	
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
