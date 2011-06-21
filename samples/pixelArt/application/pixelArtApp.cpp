#include "pixelArtApp.h"
#include "application/poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new pixelArtApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

pixelArtApp::pixelArtApp() {
	addModifier(new poCamera2D(poColor::black));
}

pixelArtApp::~pixelArtApp() {
}