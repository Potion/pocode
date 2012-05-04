/////////////////////////////////////////
//
// pocode : Advanced Shapes
//
/////////////////////////////////////////

#include "AdvancedShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"							// Include poImageShape.h to be able to use poShapes
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes


// APP CONSTRUCTOR. Create all objects here.
AdvancedShapesApp::AdvancedShapesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. poShape2D ///////////////////////
	
	poShape2D* A = new poShape2D();							// Create a poShape2D
	A->addPoint(-40,-10);									// Add a point to the shape, x and y coordinates
	A->addPoint(50,-50);
	A->addPoint(30,40);
	A->addPoint(-20,30);
    A->fillColor = poColor::orange;
	A->position.set(120, 240, 0);
	addChild( A );
	
	
	// B. poShape2D using math ///////////////////////
	
	poShape2D* B = new poShape2D();							// Create a poShape2D
	
	for ( int i=0 ; i<=120 ; i++ ) {   
        
        float dirX,dirY;									// Set variables to control the progression
        float scale = 0.05;									// of the mathematical formula
        int val;
		
        if (i<=60) {
            dirX = -1;
            dirY = -1;
            val = i;
        } 
        else {
            dirX = 1;
            dirY = -1;
            val = (int)(i-60);
        } 
													// Mathematical equation to calculate x and y of a point
        float x = dirX*scale*(-powf(val, 2)+40*val+1200)*sin(M_PI*val/180); 
        float y = dirY*scale*(-powf(val, 2)+40*val+1200)*cos(M_PI*val/180);
		
        poPoint P = poPoint(x, y);					// Create the poPoint
		
        B->addPoint(P);								// Add the poPoint to the shape B
    }
    B->fillColor = poColor::orange;
	B->position.set(310, 280, 0);
	addChild( B );
	
	
	// C. poStarShape ///////////////////////
	
	poShape2D* C = new poShape2D();					// Create a poShape2D
	
	C->addPoint(0,0);								// Add a point to the shape C
	C->curveTo(poPoint(50,-50), poPoint(50,0));		// curveTo() allows you to define a curve when drawing a shape
	C->curveTo(poPoint(0,0), poPoint(0,-50));		// Define an end point and control point to describe the curve
	C->curveTo(poPoint(-50,-50), poPoint(0,-50));	// Alternatively define a curve sergment using 2 control points: 
	C->curveTo(poPoint(0,0), poPoint(-50,0));		// curveTo( poPoint pt, poPoint control1, poPoint control2 )
	C->curveTo(poPoint(-50,50), poPoint(-50,0));
	C->curveTo(poPoint(0,0), poPoint(0,50));
	C->curveTo(poPoint(50,50), poPoint(0,50));
	C->curveTo(poPoint(0,0), poPoint(50,0));
	
	C->fillColor = poColor::orange;
	C->position.set(490, 240, 0);
	addChild( C );
	
	
    // D. poShape2D from a SVG file ///////////////////////
	
	poShape2D* D;
	
	std::vector<poShape2D*> shapes = createShapesFromSVGfile("mouth.svg");	// createShapesFromSVGfile(file.svg) 
																			// returns a std::vector list of 
																			// poShape2D pointers
	
	D = shapes.back();					// Since we know there is only 1 shape, we can get only the last one
	D->fillColor = poColor::orange;
    D->position.set(600, 190, 0);
    addChild( D );
}

// APP DESTRUCTOR. Delete all objects here.
AdvancedShapesApp::~AdvancedShapesApp() {
}


// UPDATE. Called once per frame. Animate objects here.
void AdvancedShapesApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void AdvancedShapesApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void AdvancedShapesApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void AdvancedShapesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
