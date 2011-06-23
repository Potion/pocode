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
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 500, 500);
}

void cleanupApplication() {
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
	
	poShape2D *shp = new poRectShape(100,100);
	shp->fillColor(poColor::red).position(100,100).name("A");
	shp->addEvent(PO_MOUSE_ENTER_EVENT, this);
	shp->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	addChild(shp);
	
	shp = new poRectShape(100,100);
	shp->fillColor(poColor::green).position(150,150).name("B");
	shp->addEvent(PO_MOUSE_ENTER_EVENT, this);
	shp->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	addChild(shp);
}

void TestObj::draw() {
}

void TestObj::update() {
}

poColor col4obj(poObject *parent, poObject *kid) {
	if(parent->getChild(0) == kid)
		return poColor::red;
	else
		return poColor::green;
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_ENTER_EVENT:
			((poShape2D*)event->source)->fill_color_tween
				.set(poColor::white)
				.setTweenFunction(linearFunc)
				.setDuration(1)
				.start();
			break;
		case PO_MOUSE_LEAVE_EVENT:
			((poShape2D*)event->source)->fill_color_tween
				.set(col4obj(this,event->source))
				.setTweenFunction(linearFunc)
				.setDuration(1)
				.start();
			break;
	}
}


