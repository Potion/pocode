/////////////////////////////////////////
//
// poCode : Advanced Shapes
//
/////////////////////////////////////////

#include "AdvancedShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"


// APP CONSTRUCTOR.
// Create all objects here.
AdvancedShapesApp::AdvancedShapesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. poShape2D ///////////////////////
	
	poShape2D* A;
    A = new poShape2D();
	A->addPoint(-40,-10);
	A->addPoint(50,-50);
	A->addPoint(30,40);
	A->addPoint(-20,30);
    A->fillColor = poColor::orange;
	A->position.set(120, 240, 0);
	addChild( A );
	
	
	// B. poShape2D using math ///////////////////////
	
	poShape2D* B;
    B = new poShape2D();
	for ( int i=0 ; i<=120 ; i++ ) {   
        
		// Set veriables to control the progression of the mathematical formula          
        float dirX,dirY;
        float scale = 0.05;
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
		
		// poPoint is a vertex in poCode
        poPoint P = poPoint(x, y);
		
        // add point to the poShape2D
        B->addPoint(P);
    }
    B->fillColor = poColor::orange;
	B->position.set(310, 280, 0);
	addChild( B );
	
	
	// C. poStarShape ///////////////////////
	
	poShape2D* C;
	C = new poShape2D();
	
	C->addPoint(0,0);
	
	// curveTo() allows you to define a curve when drawing a shape
	// Define an end point and control point to describe the curve
	// Alternatively a curve sergment can be defined using two control points: 
	// curveTo( poPoint pt, poPoint control1, poPoint control2 )
	C->curveTo(poPoint(50,-50), poPoint(50,0));
	C->curveTo(poPoint(0,0), poPoint(0,-50));
	C->curveTo(poPoint(-50,-50), poPoint(0,-50));
	C->curveTo(poPoint(0,0), poPoint(-50,0));
	C->curveTo(poPoint(-50,50), poPoint(-50,0));
	C->curveTo(poPoint(0,0), poPoint(0,50));
	C->curveTo(poPoint(50,50), poPoint(0,50));
	C->curveTo(poPoint(0,0), poPoint(50,0));
	
	C->fillColor = poColor::orange;
	C->position.set(490, 240, 0);
	addChild( C );
	
	
    // D. poShape2D from a SVG file ///////////////////////
	
	poShape2D* D;
	
	// createShapesFromSVGfile(file.svg) returns a std::vector list of poShape2D pointers
	std::vector<poShape2D*> shapes = createShapesFromSVGfile("house.svg");
	
	// Since we know there is only 1 shape, we can decide to get only the last one
	D = shapes.back();
	D->fillColor = poColor::orange;
    D->position.set(630, 190, 0);
    addChild( D );
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
AdvancedShapesApp::~AdvancedShapesApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void AdvancedShapesApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void AdvancedShapesApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AdvancedShapesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
