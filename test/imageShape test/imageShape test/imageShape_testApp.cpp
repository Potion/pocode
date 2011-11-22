#include <iostream>
#include "poCamera.h"
#include "poHelpers.h"
#include "poImageShape.h"
#include "poApplication.h"
#include "imageShape_testApp.h"

poObject *createObjectForID(uint uid) {
	return new imageShape_testApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "imageShape_test", 100, 100, 250, 250);
}

void cleanupApplication() {
}

imageShape_testApp::imageShape_testApp() {
	setCurrentPath(applicationGetResourceDirectory());
	setWindowMouseMoveEnabled(false);

	addModifier(new poCamera2D());

	poImageShape *shp = new poImageShape("test.png");
	shp->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	shp->setAlignment(PO_ALIGN_CENTER_CENTER);
	shp->position = getWindowCenter();
	addChild(shp);
}

imageShape_testApp::~imageShape_testApp() {
}

void imageShape_testApp::eventHandler(poEvent *event) {
	std::cout << "click" << std::endl;
}

void imageShape_testApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
