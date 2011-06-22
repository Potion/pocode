#include "SlideShowApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new SlideShowApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SlideShow", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

SlideShowApp::SlideShowApp() {
	addModifier(new poCamera2D(poColor::black));
}

SlideShowApp::~SlideShowApp() {
}

void SlideShowApp::eventHandler(poEvent *event) {
	
}

void SlideShowApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
