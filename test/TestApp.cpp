#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

#include <tinyxml.h>

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 1050, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier(new poCamera2D());
	poRectShape *shape = new poRectShape("images/alfred_e_neuman.jpg");
	addChild(shape);
	
	shape->fill_color_tween
		.set(poColor::black)
		.setTweenFunction(linearFunc)
		.setDuration(1.f)
		.setRepeat(PO_TWEEN_REPEAT_PINGPONG)
		.start();
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}
