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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -500, 400, 850, 850);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
	poShape2D *shape = new poRectShape("images/108_Dry Dock 5_regular.png");
	shape->alignment(PO_ALIGN_CENTER_RIGHT);
	shape->drawBounds(true);
	shape->alphaTestTextures(true);
	shape->position(425,425);
	addChild(shape);
	
	shape->addEvent(PO_MOUSE_PRESS_EVENT, this);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	printf("clicked: %.2f %.2f\n", event->local_position.x, event->local_position.y);
}
