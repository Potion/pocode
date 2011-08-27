#include "DropShadowApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "helpers.h"

poObject *createObjectForID(uint uid) {
	return new DropShadowApp();
}

void setupApplication() {
	
	// set resources path
	fs::path path;
	pathToFolder("resources", &path);
	setCurrentPath(path/"resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "DropShadow", 100, 100, 600, 300);
}

void cleanupApplication() {
}

DropShadowApp::DropShadowApp() {
	addModifier(new poCamera2D(poColor::dk_grey));
	
	
	item = new poRectShape(100.f, 100.f);
	shadow = new poDropShadow(item->bounds.size, 50.f, .5f);
	shadow->position = poPoint( 250.f, 100.f );
	shadow->addChild(item);
	addChild(shadow);
	
	addEvent(PO_MOUSE_DRAG_EVERYWHERE_EVENT, this);
}

DropShadowApp::~DropShadowApp() {
}

void DropShadowApp::eventHandler(poEvent *event) {
	
	shadow->setAlpha(event->position.x / 600.f);
	shadow->setSpread(item->bounds.size, event->position.y / 3.f);
}

void DropShadowApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
