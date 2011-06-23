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
	log("closing application");
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
//	poShape2D *shape = new poRectShape(150,150);
//	shape->alignment(PO_ALIGN_CENTER_CENTER);
//	shape->position(getWindowWidth()/2.f, getWindowHeight()/2.f);
//	addChild(shape);
//	
//	poGUIPointer *pointer = new poGUIPointer("position", poPoint(), poRect(0,0,getWindowWidth(),getWindowHeight()), boost::bind(&poObject::position, shape, _1));
//	addChild(pointer);
	
	poTextBox *tb = new poTextBox(200,200);
	tb->text("hello world").layout();
	addChild(tb);
	
	log("made a text box (%s)",tb->text().c_str());
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}


