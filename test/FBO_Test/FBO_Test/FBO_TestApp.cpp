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
	shp->placeTexture(poTexture(getImage("pitbull.jpeg")));
	shp->position.set(50,50,0);
	shp->addModifier(fbo);
	addChild(shp);
	
	poRectShape *shp2 = new poRectShape(500,500);
	shp2->placeTexture(fbo->colorTexture());
	shp2->position.set(50,50,0);
	addChild(shp2);

	poRectShape *shp3 = new poRectShape(400,400);
	shp3->placeTexture(poTexture(getImage("pitbull.jpeg")));
	shp3->position.set(650,50,0);
	addChild(shp3);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void FBO_TestApp::draw() {
	if(fancyDraw) {
		std::vector<poPoint> grid;
		std::vector<poPoint> coords;
		std::vector<unsigned short> indices;
		
		int dim = 10;
		
		for(int i=0; i<=dim; i++) {
			for(int j=0; j<=dim; j++) {
				float pctj = j / float(dim);
				float pcti = i / float(dim);
				
				grid.push_back(poPoint(pctj*500.f, pcti*500.f, 0.f) + poPoint(50,50));
				coords.push_back(poPoint(pctj, 1.0-pcti, 0.f));
				
				indices.push_back(i*10 + j);
				indices.push_back((i+1)*10 + j);
				indices.push_back((i+1)*10 + j+1);
				indices.push_back(i*10 + j+1);
			}
		}
		
		for(int i=0; i<grid.size(); i++) {
			float r = poRand(0,6);
			grid[i].x += r;
			grid[i].y += r;
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

