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
	applicationCreateWindow(0, WINDOW_TYPE_FULLSCREEN, "TestObj", 100, 100, 1050, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	for(int i=0; i<5; i++) {
		poRectShape *shape = new poRectShape(150,150);
		shape->position(i*155+50, 200);
		shape->fillColor(poColor::white);
		shape->fill_color_tween
			.set(poColor::green)
			.setTweenFunction(linearFunc)
			.setDuration(2.f)
			.setDelay(i)
			.setRepeat(PO_TWEEN_REPEAT_PINGPONG)
			.start();
		addChild(shape);
	}
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}
