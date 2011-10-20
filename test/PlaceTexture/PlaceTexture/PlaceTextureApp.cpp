#include "poResourceLoader.h"
#include "PlaceTextureApp.h"
#include "poShapeBasics2D.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

poObject *createObjectForID(uint uid) {
	return new PlaceTextureApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "PlaceTexture", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

PlaceTextureApp::PlaceTextureApp() {
	addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	shape = addChild(new poRectShape(200,400));
	shape->generateStroke(5);
	shape->strokeColor = poColor::red;
	
}

PlaceTextureApp::~PlaceTextureApp() {
}

void PlaceTextureApp::eventHandler(poEvent *event) {
	static bool toggle = false;
	static poTexture *tex = NULL;

	toggle = !toggle;

	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			if(tex)
				delete tex;
			
			poImage *img = getImage(toggle ? "pitbull.jpeg" : "news.jpeg");
			img->changeChannelCount(4);
			tex = new poTexture(img);
			
			shape->placeTexture(tex, PO_TEX_FIT_INSIDE);
			
		}
		else 
		if(event->keyChar == 't') {
			if(toggle)
				shape->reshape(200, 400);
			else
				shape->reshape(400, 200);
			
			if(!tex)
				return;

			shape->placeTexture(tex, PO_TEX_FIT_INSIDE);
		}
	}
}

void PlaceTextureApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}



