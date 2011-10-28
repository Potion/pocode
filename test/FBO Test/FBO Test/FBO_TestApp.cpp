#include "poFBO.h"
#include "Helpers.h"
#include "poCamera.h"
#include "FBO_TestApp.h"
#include "poApplication.h"
#include "SimpleDrawing.h"
#include "poShapeBasics2D.h"
#include "poResourceLoader.h"

poObject *createObjectForID(uint uid) {
	lookUpAndSetPath("resources");
	return new FBO_TestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "FBO_Test", 100, 100, 1200, 600);
}

void cleanupApplication() {
}

FBO_TestApp::FBO_TestApp() {
	fancyDraw = false;
	
	addModifier(new poCamera2D());
	fbo = new poFBO(500,500,poFBOConfig().setNumMultisamples(4));
	
	poRectShape *shp = new poRectShape(400,400);
	shp->generateStroke(10, PO_STROKE_PLACE_CENTER, PO_STROKE_JOIN_ROUND);
	shp->placeTexture(poTexture(getImage("pitbull.jpeg")));
	shp->position.set(50,50,0);
	shp->addModifier(fbo);
	addChild(shp);
	
	poRectShape *shp2 = new poRectShape(500,500);
	shp2->placeTexture(fbo->colorTexture());
	shp2->position.set(50,50,0);
	addChild(shp2);

	poRectShape *shp3 = new poRectShape(400,400);
	shp3->generateStroke(10, PO_STROKE_PLACE_CENTER, PO_STROKE_JOIN_ROUND);
	shp3->placeTexture(poTexture(getImage("pitbull.jpeg")));
	shp3->position.set(650,50,0);
	addChild(shp3);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void FBO_TestApp::draw() {
	if(fancyDraw) {
		std::vector<poPoint> grid(100);
		std::vector<poPoint> coords(100);
		std::vector<int> indices(100*4);
		for(int i=0; i<10; i++) {
			for(int j=0; j<10; j++) {
				grid[i*10+j].set(j/10.f*500.f, i/10.f*500.f, 0.f);
				coords[i*10+j].set(j/10.f, i/10.f, 0.f);
			}
		}
		
		for(int i=0; i<100*4; i++) {
			grid[i].x += poRand(0,6);
			grid[i].y += poRand(0,6);
		}
		
		po::drawPoints(grid, indices, fbo->colorTexture(), coords, GL_QUADS);
	}
}

void FBO_TestApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			fancyDraw = !fancyDraw;
			for(int i=1; i<3; i++)
				getChild(i)->visible = !fancyDraw;
		}
	}
}

void FBO_TestApp::messageHandler(const std::string &msg, const poDictionary& dict) {	
}

