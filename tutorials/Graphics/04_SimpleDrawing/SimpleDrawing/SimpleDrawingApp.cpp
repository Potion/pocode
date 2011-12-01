/////////////////////////////////////////
//
// LESSON : Simple drawing
//
/////////////////////////////////////////

#include "SimpleDrawingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poSimpleDrawing.h"


// APP CONSTRUCTOR.
// Create all objects here.
SimpleDrawingApp::SimpleDrawingApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
//    BG->alpha = 0.5;
//	addChild( BG );
	
	bgTex = new poTexture("bg.jpg");
	rectTex = new poTexture("pear.jpg");
	polygonTex = new poTexture("orange.jpg");
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
SimpleDrawingApp::~SimpleDrawingApp() {
}


// DRAW
// Draw directly here. This is called to draw everything.
void SimpleDrawingApp::draw() {
	
	// Show poCode lesson image in the background
	po::setColor(poColor::white);
	po::drawTexturedRect(bgTex, 0, 0, 800, 600);
	
	
	// A. Draw lines ///////////////////////
	
	float Ax, Ay, Bx, By;
	for (int i=0; i < (150/2.5); i++) {
		
		if (i < (150/5)) {
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
		
		po::setColor(poHSVColor(1/(150/2.5) + i * (1/(150/2.5)), 1, 1));
		po::drawLine(poPoint(Ax, Ay), poPoint(Bx, By));
	}
	
	
	// B. Draw rectangles ///////////////////////
	
	po::setColor(poColor(0.2, 0.6, 0.2));
	
	po::drawStrokedRect(305, 175, 120, 120);
	
	po::drawFilledRect(335, 205, 120, 120);
	
	po::setColor(poColor::white);
	po::drawTexturedRect(rectTex, 365, 235, 120, 120);
	
	
	// C. Draw polygons ///////////////////////
	
	po::setColor(poColor::orange);
	
	poPoint A(620, 175);
	poPoint B(700, 235);
	poPoint C(670, 315);
	poPoint D(560, 295);
	poPoint E(545, 235);
	
	std::vector<poPoint> points;
	points.push_back(A);
	points.push_back(B);
	points.push_back(C);
	points.push_back(D);
	points.push_back(E);
	
	po::drawStrokedPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,25);
	}
	
	po::drawFilledPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,25);
	}
	
	po::setColor(poColor::white);
	po::drawTexturedPolygon(points, polygonTex, PO_TEX_FIT_INSIDE, PO_ALIGN_CENTER_CENTER);
	
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void SimpleDrawingApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void SimpleDrawingApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void SimpleDrawingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
