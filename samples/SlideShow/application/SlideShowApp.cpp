#include "SlideShowApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new SlideShowApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SlideShow", 100, 100, 600, 300);
}

void cleanupApplication() {
}

SlideShowApp::SlideShowApp() {
	addModifier(new poCamera2D(poColor::black));
	
	active = NULL;
	numImages = 10;
	current = 0;
	
	loadImages();
	addImages();
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void SlideShowApp::loadImages(){
	for(int i=0; i<numImages; i++){
		images.push_back(new poRectShape(500,200));
		images.back()->position(getWindowWidth()/2, getWindowHeight()/2);
		images.back()->alignment(PO_ALIGN_CENTER_CENTER);
		images.back()->fillColor(hashPointerForColor(images.back()));
	}
}

void SlideShowApp::addImages(){
	for(int i=0; i<images.size(); i++){
		addChild(images[i]);
		images[i]->visible(false);
	}
}

void SlideShowApp::update(){

}

SlideShowApp::~SlideShowApp() {
}

void SlideShowApp::eventHandler(poEvent *event) {
	if(current<images.size()){
		if(active) {
			active->visible(false);
		}
		
		active = images[current++];
		active->visible(true);
	
	}else{
		current=0;
	}
}

void SlideShowApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
