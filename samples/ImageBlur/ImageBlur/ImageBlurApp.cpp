#include "ImageBlurApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

poObject *createObjectForID(uint uid) {
	return new ImageBlurApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "ImageBlur", 100, 100, 500, 500);
}

void cleanupApplication() {
}

ImageBlurApp::ImageBlurApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poImage *img = getImage("check.jpeg");

	poImage *blur = img->copy();
	blur->blur(3,2);

	poObject *obj;
	
	obj= addChild(new poRectShape(img->texture()));
	obj->alignment(PO_ALIGN_CENTER_RIGHT);
	obj->position = getWindowCenter();

	obj = addChild(new poRectShape(blur->texture()));
	obj->alignment(PO_ALIGN_CENTER_LEFT);
	obj->position = getWindowCenter();
	
}

ImageBlurApp::~ImageBlurApp() {
}

void ImageBlurApp::eventHandler(poEvent *event) {
	
}

void ImageBlurApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
