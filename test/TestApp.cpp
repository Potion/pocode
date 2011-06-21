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

poShape2D *img = NULL;
poShape2D *shape = NULL;

TestObj::TestObj() {
	addModifier(new poCamera2D());
	addEvent(PO_WINDOW_RESIZED_EVENT, this);
	
	img = new poRectShape("images/alfred_e_neuman.jpg");
	img->addEvent(PO_MOUSE_MOVE_EVENT, this);
	addChild(img);
	
	addChild(shape = new poRectShape(50,50));
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_WINDOW_RESIZED_EVENT:
			shape->position(getWindowWidth()-50, getWindowHeight()-50);
			break;
			
		case PO_MOUSE_MOVE_EVENT:
		{
			poColor color = img->texture()->colorAtPoint(event->local_position);
			shape->fillColor(color);
			break;
		}
	}
}


