#include "MasksApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new MasksApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

MasksApp::MasksApp() {
	addModifier(new poCamera2D(poColor::black));
}

MasksApp::~MasksApp() {
}