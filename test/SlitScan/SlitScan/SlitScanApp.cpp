#include "SlitScanApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new SlitScanApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SlitScan", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

SlitScanApp::SlitScanApp() {
	addModifier(new poCamera2D(poColor::black));
}

SlitScanApp::~SlitScanApp() {
}

void SlitScanApp::eventHandler(poEvent *event) {
	
}

void SlitScanApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
