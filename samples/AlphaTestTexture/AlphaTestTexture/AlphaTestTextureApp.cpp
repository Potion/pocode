#include "AlphaTestTextureApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new AlphaTestTextureApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AlphaTestTexture", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

AlphaTestTextureApp::AlphaTestTextureApp() {
	addModifier(new poCamera2D(poColor::black));
}

AlphaTestTextureApp::~AlphaTestTextureApp() {
}

void AlphaTestTextureApp::eventHandler(poEvent *event) {
	
}

void AlphaTestTextureApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
