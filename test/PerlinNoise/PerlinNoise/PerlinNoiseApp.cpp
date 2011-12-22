#include "PerlinNoiseApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new PerlinNoiseApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "PerlinNoise", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

PerlinNoiseApp::PerlinNoiseApp() {
	addModifier(new poCamera2D(poColor::black));
	
	p = new Perlin(4,4,1,94);	// initialize an instance of perlin noise
	
	r = new poRectShape(getWindowWidth(), getWindowHeight());	// container for texture
	
	img = new poImage("cyan.png");	// placeholder to be written to as texture
	
	// set every pixel of the above placeholder texture with a perlin value for its location
	for(int j = 0; j<getWindowHeight(); j++){
		for(int i = 0; i<getWindowWidth(); i++){
			img->setPixel(poPoint(i,j), poColor((p->Get(i/getWindowWidth(), j/getWindowHeight())+1)/2, 
												(p->Get(i/getWindowWidth(), j/getWindowHeight())+1)/2, 
												(p->Get(i/getWindowWidth(), j/getWindowHeight())+1)/2));
		}
	}
	
	r->placeTexture(new poTexture(img));	// place the newly rewritten texture
	
	addChild(r);
}

PerlinNoiseApp::~PerlinNoiseApp() {
}

void PerlinNoiseApp::eventHandler(poEvent *event) {
	
}

void PerlinNoiseApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
