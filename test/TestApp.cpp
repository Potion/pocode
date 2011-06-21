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
	
	poRectShape *shape = new poRectShape("images/kittens.jpeg");
	shape->addEvent(PO_MOUSE_PRESS_EVENT, this);
	addChild(shape);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	poShape2D *shape = static_cast<poShape2D*>(event->source);
	
	static bool alfred = true;
	if(alfred) {
		poTexture tex("images/alfred_e_neuman.jpg");
		shape->placeTexture(&tex, PO_TEX_FIT_V);
	}
	else {
		poTexture tex("images/kittens.jpeg");
		shape->placeTexture(&tex);
	}
	
	alfred = !alfred;
}


