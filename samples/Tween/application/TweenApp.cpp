#include "TweenApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new TweenApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Tween", 100, 100, 600, 300);
}

void cleanupApplication() {
}

TweenApp::TweenApp() {
	addModifier(new poCamera2D(poColor::black));
}

TweenApp::~TweenApp() {
}

void TweenApp::eventHandler(poEvent *event) {
	
}

void TweenApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
