#include "KinectTouchSampleApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new KinectTouchSampleApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "KinectTouchSample", 100, 100, 640, 480);
}

void cleanupApplication() {
}

KinectTouchSampleApp::KinectTouchSampleApp() {
	addModifier(new poCamera2D(poColor::black));
}

KinectTouchSampleApp::~KinectTouchSampleApp() {
}

void KinectTouchSampleApp::eventHandler(poEvent *event) {
	
}

void KinectTouchSampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
