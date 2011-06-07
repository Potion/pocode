#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 800, 800);
}

void cleanupApplication() {
}

poTexture *tex = NULL;
bool inverted = false;

TestObj::TestObj() {
    addModifier(new poCamera2D(poColor::black,false));
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TestObj::eventHandler(poEvent *event) {
	printf("%d %s\n", event->keyCode, isArrowKey(event->modifiers)?"true":"false");
}
