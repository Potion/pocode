#include "___PROJECTNAMEASIDENTIFIER___App.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new ___PROJECTNAMEASIDENTIFIER___App();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "___PROJECTNAMEASIDENTIFIER___", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

___PROJECTNAMEASIDENTIFIER___App::___PROJECTNAMEASIDENTIFIER___App() {
	addModifier(new poCamera2D(poColor::black));
}

___PROJECTNAMEASIDENTIFIER___App::~___PROJECTNAMEASIDENTIFIER___App() {
}

void ___PROJECTNAMEASIDENTIFIER___App::eventHandler(poEvent *event) {
	
}

void ___PROJECTNAMEASIDENTIFIER___App::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
