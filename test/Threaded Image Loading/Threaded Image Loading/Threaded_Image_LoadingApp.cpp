#include "Threaded_Image_LoadingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

poObject *createObjectForID(uint uid) {
	return new Threaded_Image_LoadingApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Threaded_Image_Loading", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

Threaded_Image_LoadingApp::Threaded_Image_LoadingApp() {
	addModifier(new poCamera2D());
	
	for(int i=0; i<5; i++) {
		loader.loadImage("Fang.jpg", this);
	}
}

Threaded_Image_LoadingApp::~Threaded_Image_LoadingApp() {
}

void Threaded_Image_LoadingApp::update() {
	loader.update();
}

void Threaded_Image_LoadingApp::eventHandler(poEvent *event) {
	
}

void Threaded_Image_LoadingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	poImage *img = dict.getPtr<poImage>("image");
	std::cout << msg << ": " << img->url() << "(" << dict.getFloat("elapsed") << ")" << std::endl;
	delete img;
}
