#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -1680, 0, 800, 1200);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	addEvent(PO_WINDOW_RESIZED_EVENT, this);

	poObject *holder = new poObject();
	holder->scale(.9, .9);
	addChild(holder);

	poShape2D *img = new poRectShape("images/testimg.png");
	img->alphaTestTextures(true)
		.fillColor(poColor::blue)
		.offset(poPoint(-100,-100))
		.rotation(25)
		.name("1");
	img->addEvent(PO_MOUSE_ENTER_EVENT, this);
	img->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	holder->addChild(img);
	
	img = new poRectShape("images/testimg.png");
	img->alphaTestTextures(true)
		.fillColor(poColor::green)
		.alignment(PO_ALIGN_CENTER_CENTER)
		.rotation(-80)
		.name("2");
	img->addEvent(PO_MOUSE_ENTER_EVENT, this);
	img->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	holder->addChild(img);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_WINDOW_RESIZED_EVENT:
			event->source->getChild(0)->getChild(0)->position(getWindowWidth()/3, getWindowHeight()/3);
			event->source->getChild(0)->getChild(1)->position(2*getWindowWidth()/3, 2*getWindowHeight()/3);
			break;
			
		case PO_MOUSE_ENTER_EVENT:
			printf("enter %s\n", event->source->name().c_str());
			break;
			
		case PO_MOUSE_LEAVE_EVENT:
			printf("leave %s\n", event->source->name().c_str());
			break;
	}
}


