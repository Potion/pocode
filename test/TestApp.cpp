#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -500, 400, 800, 800);
}

void cleanupApplication() {
}

poShape2D *shape = NULL;
poShape2D *tester = NULL;

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
	shape = new poRectShape("images/108_Dry Dock 5.png");
	shape->drawBounds(true);
	shape->setAlignment(PO_ALIGN_TOP_LEFT);
	shape->alphaTestTextures(true);
	shape->addEvent(PO_MOUSE_MOVE_EVENT, this);
	shape->position(400,400);
	addChild(shape);
	
	tester = new poRectShape(100,100);
	addChild(tester);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	poColor color = shape->texture()->colorAtPoint(event->local_position);
	tester->fillColor(color);
}
