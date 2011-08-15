#include "AdvancedTweensApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedTweensApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedTweens", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

AdvancedTweensApp::AdvancedTweensApp() {
	addModifier(new poCamera2D(poColor::black));
}

AdvancedTweensApp::~AdvancedTweensApp() {
}

void AdvancedTweensApp::eventHandler(poEvent *event) {
	
}

void AdvancedTweensApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
