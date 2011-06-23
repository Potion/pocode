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

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
	poShape2D *shape = new poRectShape(150,150);
	shape->alignment(PO_ALIGN_CENTER_CENTER);
	shape->position(getWindowWidth()/2.f, getWindowHeight()/2.f);
	addChild(shape);
	
	poObject *gui = new poObject();
	gui->position(5,5);
	addChild(gui);
	
	poGUIPointer *pointer = new poGUIPointer("position", shape->position(), poRect(0,0,getWindowWidth(),getWindowHeight()), 0, 100, boost::bind(&poObject::position, shape, _1));
	gui->addChild(pointer);
	
	poGUISlider *slider = new poGUISlider("rotation", 0, 0, 360, 1, boost::bind(&poObject::rotation,shape,_1));
	slider->position(pointer->position() + poPoint(0, pointer->bounds().height()+5));
	gui->addChild(slider);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}


