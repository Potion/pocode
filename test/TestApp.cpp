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
	
	poShape2D *shape = NULL;
	
	shape = new poRectShape("images/testimg.png");
	addChild(shape);
	
	shape = new poRectShape("images/108_Dry Dock 5_regular.png");
	addChild(shape);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_WINDOW_RESIZED_EVENT)
		cout << "resized " << getWindowBounds() << endl;
	else
		cout << "clicked " << event->message << endl;
}


