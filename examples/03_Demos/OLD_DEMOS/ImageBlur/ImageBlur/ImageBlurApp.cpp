#include "ImageBlurApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "SimpleDrawing.h"
#include "poShapeBasics2D.h"
#include "poResourceLoader.h"

poObject *createObjectForID(uint uid) {
	return new ImageBlurApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "ImageBlur", 100, 100, 900, 900);
}

void cleanupApplication() {
}

ImageBlurApp::ImageBlurApp() {
	addModifier(new poCamera2D(poColor::grey));
	
	poImage img = getImage("villain.png");
	poImage chk = getImage("kittens.jpeg");
	
	poImage blur = img.copy();
	blur.blur(15,2);
	
	blur.composite(chk, poPoint(50,50), 0.5f);

	poObject *obj;
	obj= new poRectShape(poTexture(img));
	obj->setAlignment(PO_ALIGN_CENTER_RIGHT);
	obj->position = getWindowCenter();
	addChild(obj);

	obj = new poRectShape(poTexture(blur));
	obj->setAlignment(PO_ALIGN_CENTER_LEFT);
	obj->position = getWindowCenter();
	addChild(obj);
	
//	delete img;
//	delete chk;
//	delete blur;
}

ImageBlurApp::~ImageBlurApp() {
}

void ImageBlurApp::eventHandler(poEvent *event) {
	
}

void ImageBlurApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
