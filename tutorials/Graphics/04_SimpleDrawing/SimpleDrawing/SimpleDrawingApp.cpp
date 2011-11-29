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
    BG->alpha = 0.5;
//	addChild( BG );
	
	bgTex = new poTexture("bg.jpg");
	rectTex = new poTexture("pear.jpg");
	polygonTex = new poTexture("lego.jpg");
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
SimpleDrawingApp::~SimpleDrawingApp() {
}


// DRAW
// Draw directly here. This is called to draw everything.
void SimpleDrawingApp::draw() {
	
	po::setColor(poColor::white);
	po::drawTexturedRect(bgTex, 0, 0, 800, 600);
	
	
	// INTERNAL!! generateStroke not working
//	po::generateStroke(points, 10);
	
	
	// A. Draw lines ///////////////////////
	
	float Ax, Ay, Bx, By;
	for (int i=0; i < (150/2.5); i++) {
		
		if (i < (150/5)) {
			Ax = 80;
			Ay = 230 + (5*i);
			Bx = 80 + (5*i);
			By = 230;
		}
		else {
			Ax = 80 + (5*(i - 150/5));
			Ay = 380;
			Bx = 230;
			By = 230 + (5*(i - 150/5));
		}
		
		po::setColor(poHSVColor(1/(150/2.5) + i * (1/(150/2.5)), 1, 1));
		po::drawLine(poPoint(Ax, Ay), poPoint(Bx, By));
	}
	
	
	// B. Draw rectangles ///////////////////////
	
	po::setColor(poColor(0.2, 0.6, 0.2));
	
	po::drawStrokedRect(305, 210, 120, 120);
	
	po::drawFilledRect(325, 240, 120, 120);
	
	po::setColor(poColor::white);
	po::drawTexturedRect(rectTex, 345, 270, 120, 120);
	
	
	// C. Draw polygons ///////////////////////
	
	po::setColor(poColor::orange);
	
	poPoint A(620, 210);
	poPoint B(700, 270);
	poPoint C(670, 350);
	poPoint D(560, 330);
	poPoint E(545, 270);
	
	std::vector<poPoint> points;
	points.push_back(A);
	points.push_back(B);
	points.push_back(C);
	points.push_back(D);
	points.push_back(E);
	
	po::drawStrokedPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,30);
	}
	
	po::drawFilledPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,30);
	}
	
//	poRect polygonBounds;
//    
//    // must initialize rect with first point
//    if ( points.size() > 0 )
//        polygonBounds.set( points[0].x, points[0].y, 0, 0 );
//    
//    // include all other points
//    BOOST_FOREACH(poPoint &p, points) {
//        polygonBounds.include(p);
//    }
//	
//	po::drawStrokedRect(poRect(polygonBounds.x, polygonBounds.y, polygonBounds.width, polygonBounds.height));
//	
//	std::vector<poPoint> boundsTexCoords = textureFit(poRect(0, 0, polygonBounds.width, polygonBounds.height), polygonTex, PO_TEX_FIT_NONE, PO_ALIGN_BOTTOM_LEFT);
//	
//	std::vector<poPoint> texCoords;
//	for(int i=0; i < points.size(); i++) {
//		
//		float coordX = (points[i].x - polygonBounds.x) / polygonBounds.width;
//		float coordY = (points[i].y - polygonBounds.y) / polygonBounds.height;
//		
//		coordX = poMapf(0, 1, coordX, boundsTexCoords[0].x, boundsTexCoords[1].x);
//		coordY = 1 - poMapf(0, 1, coordY, boundsTexCoords[2].y, boundsTexCoords[0].y);
//		
//		texCoords.push_back(poPoint(coordX, coordY));
//	}
	
	po::setColor(poColor::white);
	drawTexturedPolygon(points, polygonTex, PO_TEX_FIT_NONE, PO_ALIGN_TOP_RIGHT);

}


void SimpleDrawingApp::drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	
	poRect bounds;
    
    // must initialize rect with first point
    if ( points.size() > 0 )
        bounds.set( points[0].x, points[0].y, 0, 0 );
    
	// include all other points
    BOOST_FOREACH(poPoint p, points) {
        bounds.include(p);
    }

	std::vector<poPoint> coords(points.size());
	textureFit(bounds, tex, fit, align, coords, points);
	po::drawTexturedPolygon(points, tex, coords);
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
