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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 400, 1050, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier(new poCamera2D());

	poImage img("images/img.jpg");
	poTexture tex(&img);
	addChild(new poRectShape(&tex));
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}
