#include "poCamera.h"
#include "pathTestApp.h"
#include "poApplication.h"
#include "poSimpleDrawing.h"


poObject *createObjectForID(uint uid) {
	return new pathTestApp();
}

void setupApplication() {
	lookUpAndSetPathNextTo("pathTest");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pathTest", 100, 100, 400, 400);
}

void cleanupApplication() {
}

pathTestApp::pathTestApp() {
	addModifier(new poCamera2D());
	addEvent(PO_MOUSE_DOWN_EVENT, this);
	
	controlHandle = poGetTexture("controlHandle.png");
	pointHandle = poGetTexture("pointHandle.png");
}

void pathTestApp::draw() {
	BOOST_FOREACH(poPath &path, paths) {
		po::drawPoints(path.generatePoints(), GL_LINE_STRIP);
	}
	BOOST_FOREACH(poPath &path, paths) {
		for(int i=0; i<path.getNumSegments(); i++) {
			
		}
	}
}

void pathTestApp::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_DOWN_EVENT:
			if(isMetaMask(event))
				paths.push_back(poPath().start(event->globalPosition));
			else
				paths.back().lineTo(event->globalPosition);
			break;
	}
}

void pathTestApp::addPoint(poPoint p) {
	if(paths.empty())
		paths.push_back(poPath().start(p));
}

void pathTestApp::movePoint(poPoint p) {
}

void pathTestApp::moveControl(poPoint p) {
}


