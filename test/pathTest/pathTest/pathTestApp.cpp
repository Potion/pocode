#include "poCamera.h"
#include "pathTestApp.h"
#include "poApplication.h"
#include "poSimpleDrawing.h"


poObject *createObjectForID(uint uid) {
	return new pathTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pathTest", 100, 100, 400, 400);
}

void cleanupApplication() {
}

pathTestApp::pathTestApp() {
	addModifier(new poCamera2D());
	addEvent(PO_MOUSE_DOWN_EVENT, this);
	addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
}

void pathTestApp::draw() {
}

void pathTestApp::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_DOWN_EVENT) {
	}
	else 
	if(event->type == PO_MOUSE_DRAG_INSIDE_EVENT) {
		
	}
}

void pathTestApp::addPoint(poPoint p) {
	
}

void pathTestApp::movePoint(poPoint p) {
	
}

void pathTestApp::moveControl(poPoint p) {
	
}


