#include "BasicShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"

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
	
	// Make a rectangular shape
    rectShape = new poRectShape( 80,70 );
    rectShape->fillColor = poColor::yellow;			// Set the color, yellow
	rectShape->generateStroke(2);					// Create a 2 pixel stroke around it
	rectShape->strokeColor.set(0.8, 0.2, 0.5);		// Set the stroke color
	rectShape->position.set( 307, 210, 0 );			// Set the position (x, y, z)
	addChild( rectShape );							// Add the object, so it gets drawn on the screen
	
	// Make an oval shape
	ovalShape = new poOvalShape( 130,130, 50 );		// 50 is the number of vertices to draw the circle
    ovalShape->fillColor.set255(50, 180, 50);			// Set RGB color
    ovalShape->position.set( 495, 210, 0 );
    addChild( ovalShape );
	
    // Make a rectangular shape with an image
    rectShapeWithImage = new poRectShape( "apple.jpg" );
    rectShapeWithImage->position.set( 245, 340, 0 );
    rectShapeWithImage->scale.set( 0.5, 0.5, 1 );	// Scale the object in 3 dimensions (x, y, z)
    addChild( rectShapeWithImage );
	
	// Make a custom shape, made of 4 vertices
    customShape = new poShape2D();
    customShape->addPoint( -30, -40 );				// Add a vertex to the shape, a poPoint
    customShape->addPoint( 60, -50 );				// The coordinates are relative to the origin point of the shape
    customShape->addPoint( 50, 30 );				// For more on poPoints see poPoint.h
    customShape->addPoint( -50, 40 );
    customShape->fillColor = poColor::orange;
    customShape->position.set( 490, 390, 0 );
    addChild( customShape );
    
	
	// setAlignment() changes the origin point of the poShape
	// Uncomment one of the following lines to modify rectShape's alignment

//	rectShape->setAlignment(PO_ALIGN_TOP_LEFT);		// Default value
//	rectShape->setAlignment(PO_ALIGN_TOP_CENTER);	// Top origin makes the shape move to the bottom
//	rectShape->setAlignment(PO_ALIGN_TOP_RIGHT);	// Right origin point makes the shape move to the left
//	rectShape->setAlignment(PO_ALIGN_CENTER_LEFT);
	rectShape->setAlignment(PO_ALIGN_CENTER_CENTER);
//	rectShape->setAlignment(PO_ALIGN_CENTER_RIGHT);
//	rectShape->setAlignment(PO_ALIGN_BOTTOM_LEFT);
//	rectShape->setAlignment(PO_ALIGN_BOTTOM_CENTER);
//	rectShape->setAlignment(PO_ALIGN_BOTTOM_RIGHT);
}

BasicShapesApp::~BasicShapesApp() {
}

// update() is where you change things at every frame
// This functions gets called every loop before the draw() function
void BasicShapesApp::update() {
	
	// increasing the rotation on a poShape2D makes it rotate clock-wise
	// rotation is expressed in degrees
	customShape->rotation += 3;
}

void BasicShapesApp::eventHandler(poEvent *event) {
	
}

void BasicShapesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
