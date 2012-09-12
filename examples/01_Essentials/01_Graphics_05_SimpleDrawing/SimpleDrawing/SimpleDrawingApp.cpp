/////////////////////////////////////////
//
// pocode : Simple drawing
//
/////////////////////////////////////////

#include "SimpleDrawingApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poSimpleDrawing.h"							// Include poImageShape.h to be able to use poSimpleDrawing
#include "poOpenGLState.h"


// APP CONSTRUCTOR. Create all objects here.
SimpleDrawingApp::SimpleDrawingApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
	bgTex = new poTexture("bg.jpg");					// Define a texture for the background
	rectTex = new poTexture("pear.jpg");				// Define a texture for the rectangle in B
	polygonTex = new poTexture("orange.jpg");			// Define a texture for the polygon in C
}


// APP DESTRUCTOR. Delete all objects here.
SimpleDrawingApp::~SimpleDrawingApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void SimpleDrawingApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void SimpleDrawingApp::draw() {
	
	// Show poCode lesson image in the background
	po::setColor(poColor::white);						// Set the color to white to draw an image
														// with its original color (not tinted)
	po::drawTexturedRect(bgTex, 0, 0, 800, 600);		// Draw a rectangle with a texture on it
	
	
	// A. Draw lines ///////////////////////
	
	float Ax, Ay, Bx, By;								// Draw some lines that create a rectangle shape
	float numLines = (150/2.5);
	for (int i=0; i < numLines; i++) {
		
		if (i < (150/5)) {								// Define x and y coordinates of 2 points, A and B
			Ax = 80;
			Ay = 195 + (5*i);
			Bx = 80 + (5*i);
			By = 195;
		}
		else {
			Ax = 80 + (5*(i - 150/5));
			Ay = 345;
			Bx = 230;
			By = 195 + (5*(i - 150/5));
		}
		
		poPoint A( Ax, Ay );							// Create a point A
		poPoint B( Bx, By );							// Create a point B
		
		po::setColor(poHSVColor(i * (1.f/numLines), 1, 1));		// Set the color for the line
		po::drawLine( A, B );									// Draw a line from A to B
	}
	
	
	// B. Draw rectangles ///////////////////////
	
	po::setColor(poColor(0.2, 0.6, 0.2));				// Set the color for the rectangles
	
	po::drawStrokedRect(305, 175, 120, 120);			// Draw a stroked rectangle (not filled)
	
	po::drawFilledRect(335, 205, 120, 120);				// Draw a filled rectangle
	
	po::setColor(poColor::white);						// Set the color to white to draw an image
														// with its original color (not tinted)
	po::drawTexturedRect(rectTex, 365, 235, 120, 120);	// Draw a rectangle with a texture on it
	
	
	// C. Draw polygons ///////////////////////
	
	po::setColor(poColor::orange);						// Set the color for the polygons
	
	poPoint A(620, 175);								// Define 5 points to draw the polygon
	poPoint B(700, 235);
	poPoint C(670, 315);
	poPoint D(560, 295);
	poPoint E(545, 235);
	
	std::vector<poPoint> points;						// Create an array of points
	points.push_back(A);								// Store the points in the array
	points.push_back(B);
	points.push_back(C);
	points.push_back(D);
	points.push_back(E);
	
	po::drawStrokedPolygon(points);						// Draw a stroked polygon
	
	for(int i=0; i < points.size(); i++) {				// Move all the points to change the position
		points[i] += poPoint(20,25);					// of the next polygon
	}
	
	po::drawFilledPolygon(points);						// Draw a filled polygon
	
	for(int i=0; i < points.size(); i++) {				// Move all the points to change the position
		points[i] += poPoint(20,25);					// of the next polygon
	}
	
	po::setColor(poColor::white);						// Set the color to white to draw an image
														// with its original color (not tinted)
														// Draw a polygon with a texture on it
	po::drawTexturedPolygon(points, polygonTex, PO_TEX_FIT_EXACT, PO_ALIGN_CENTER_CENTER);
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void SimpleDrawingApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void SimpleDrawingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
