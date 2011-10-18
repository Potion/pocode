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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "FBO_Test", 100, 100, 600, 600);
}

void cleanupApplication() {
}

FBO_TestApp::FBO_TestApp() {
	addModifier(new poCamera2D());
	fbo = new poFBO(500,500,poFBOConfig().setNumMultisamples(4));
	
	poObject *obj = addChild(new poObject());
	obj->addModifier(fbo);
	
	poRectShape *shp = obj->addChild(new poRectShape(400,400));
	shp->position.set(50,50,0);
	shp->generateStroke(10, PO_STROKE_PLACE_CENTER, PO_STROKE_JOIN_ROUND);
	shp->placeTexture(new poTexture(getImage("pitbull.jpeg"), poTextureConfig().setMinFilter(GL_LINEAR).setMagFilter(GL_LINEAR)));
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void FBO_TestApp::draw() {
	if(fbo->enabled()) {
		po::setColor(poColor::white);
		po::drawRect(fbo->colorTexture());
	}
}

void FBO_TestApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			fbo->enabled(!fbo->enabled());
		}
	}
}

void FBO_TestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
