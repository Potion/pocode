/////////////////////////////////////////
//
// pocode : Colors
//
/////////////////////////////////////////

#include "ColorsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"						// Include poImageShape.h to be able to use poShapes


// APP CONSTRUCTOR.
// Create all objects here.
ColorsApp::ColorsApp() {

	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    FILE* Ff = fopen("bg.jpg", "r");
	if(!Ff) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(Ff);
	
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. RGB (0 to 1) ///////////////////////
	
	poColor A;
	A = poColor::red;										// Set the color using so me pre-defined poColors
															// (see the file poColor.h)
	
	poRectShape* rectA = new poRectShape(135, 75);			// Draw a rectangle
	rectA->fillColor = A;									// Assign the color A to the rectangle
	rectA->position.set(57, 173, 0);
	addChild(rectA);
	
	
	// B. RGB (0 to 1) ///////////////////////
	
	poColor B;
	B.set(1, 0.75, 0, 1);									// Set the color in RGB
															// Each value is from 0 to 1
	
	poRectShape* rectB = new poRectShape(135, 75);			// Draw a rectangle
	rectB->fillColor = B;									// Assign the color B to the rectangle
	rectB->position.set(240, 173, 0);
	addChild(rectB);
	
	
	// C. RGB (0 to 255) ///////////////////////
	
	poColor C;
	C.set255(255, 255, 0, 255);								// Set the color in RGB
															// using values from 0 to 255
	
	poRectShape* rectC = new poRectShape(135, 75);			// Draw a rectangle
	rectC->fillColor = C;									// Assign the color C to the rectangle
	rectC->position.set(423, 173, 0);
	addChild(rectC);
	
	
	// D. HSV ///////////////////////
	
	poColor D;
	D.setHSV(0.45, 1.0, 1.0);								// Set the color in HSV
															// Each value is from 0 to 1
	
	poRectShape* rectD = new poRectShape(135, 75);			// Draw a rectangle
	rectD->fillColor = D;									// Assign the color D to the rectangle
	rectD->position.set(607, 173, 0);
	addChild(rectD);
	
	
	// E. Set with a string ///////////////////////
	
	poColor E;
	E.set("#00CCFF");										// Set the color using a string,
															// following the format "#001122"
															// or "rgba(255,255,255,255)"
	
	poRectShape* rectE = new poRectShape(135, 75);			// Draw a rectangle
	rectE->fillColor = E;									// Assign the color E to the rectangle
	rectE->position.set(57, 353, 0);
	addChild(rectE);
	
	
	// F. Set individual values ///////////////////////
	
	poColor F = poColor(0, 0, 0);							// Create a color
	F.R = 0.1;												// Change the red value
	F.G = 0.1;												// Change the green value
	F.B = 1;												// Change the blue value
	F.A = 1;												// Change the alpha value
	
	poRectShape* rectF = new poRectShape(135, 75);			// Draw a rectangle
	rectF->fillColor = F;									// Assign the color F to the rectangle
	rectF->position.set(240, 353, 0);
	addChild(rectF);
	
	
	// G. Blend 2 colors ///////////////////////
	
	poColor G;
	G = blendColors(poColor::blue, poColor::magenta, 0.5);	// Define a color by blending 2 colors
															// 0.5 is the percent of the first color
															// (between 0 and 1)
	
	poRectShape* rectG = new poRectShape(135, 75);			// Draw a rectangle
	rectG->fillColor = G;									// Assign the color G to the rectangle
	rectG->position.set(423, 353, 0);
	addChild(rectG);
	
	
	// H. Random color ///////////////////////
	
	poRandSeed();											// Call this only once in your application
															// if you want to use poCode random functions
															// like poRand() or poColor::random()
	poColor H;
	H = poColor::random();									// Create a random poColor
	
	poRectShape* rectH = new poRectShape(135, 75);			// Draw a rectangle
	rectH->fillColor = H;									// Assign the color H to the rectangle
	rectH->position.set(607, 353, 0);
	addChild(rectH);
}

// APP DESTRUCTOR. Delete all objects here.
ColorsApp::~ColorsApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void ColorsApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void ColorsApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void ColorsApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void ColorsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
