#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	log("application started");
	log("path is %s", currentPath().c_str());
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 800, 500);
}

void cleanupApplication() {
	log("application ending");
}

poOrthoCamera *cam = NULL;

TestObj::TestObj() {
	addModifier((cam = new poOrthoCamera(0,0,getWindowWidth(),getWindowHeight(), -1, 1)));
//	addEvent(PO_KEY_DOWN_EVENT, this);
//	poObject *obj = createShapesFromSVGfile("images/tester.svg");
//	addChild(obj);
	
	poRectShape *rect = new poRectShape(400,400);
	rect->position(50,50);
	rect->placeTexture(poTexture("images/meatballspoon.jpg"), PO_TEX_FIT_H);
	addChild(rect);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	poRect r = cam->get();
	cout << r << endl;

	switch(event->keyCode) {
		case PO_RIGHT_ARROW:
			r.origin.x += 20;
			cam->set(r);
			break;
		case PO_LEFT_ARROW:
			r.origin.x -= 20;
			cam->set(r);
			break;
		case PO_DOWN_ARROW:
			r.origin.y += 20;
			cam->set(r);
			break;
		case PO_UP_ARROW:
			r.origin.y -= 20;
			cam->set(r);
			break;
	}
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
}



