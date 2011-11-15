#include "BasicShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poShapeBasics2D.h"				// include poShapeBasics2D.h to be able to use poShapes

poObject *createObjectForID(uint uid) {
	return new BasicShapesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicShapes", 100, 100, 800, 600);
}

void cleanupApplication() {
}

BasicShapesApp::BasicShapesApp() {
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	/////////////////////////////////////////
	//
	// LESSON 1. Basic Shapes
	//
	/////////////////////////////////////////
	
	// Make a rectangular shape
	poRectShape* A;
    A = new poRectShape(75, 75);
    A->fillColor = poColor::red;			// Set the color, red
	A->generateStroke(1);					// Create a 1 pixel stroke around it
	A->strokeColor = poColor::black;		// Set the stroke color
	A->position.set(88, 264, 0);			// Set the position (x, y, z)
	addChild( A );							// Add the object, so it gets drawn on the screen
	
	// Make an oval shape
	poOvalShape* B;
	B = new poOvalShape(100, 65, 50);		// 50 is the number of vertices to draw the circle
	B->fillColor = poColor::red;
	B->generateStroke(1);
	B->strokeColor = poColor::black;
	B->position.set(310, 300, 0);
	addChild( B );
	
	// Make a star shape
	poStarShape* C;
//	C = new poStarShape(100, 100, 20, 30);
	C = new poStarShape(50, 20, 20);
	C->fillColor = poColor::red;
	C->generateStroke(1);
	C->strokeColor = poColor::black;
	C->position.set(490, 300, 0);
	addChild( C );
	
    // Make a rectangular shape with an image
	poRectShape* D;
	D = new poRectShape( "apple.jpg" );
    D->scale.set(0.5, 0.5, 1);				// Scale the object in 3 dimensions (x, y, z)
    D->position.set(615, 250, 0);
    addChild( D );
}

BasicShapesApp::~BasicShapesApp() {
}

void BasicShapesApp::eventHandler(poEvent *event) {
	
}

void BasicShapesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
