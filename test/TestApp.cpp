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
	
	tex = new poTexture(GL_RGBA, 100, 100, 100*100*4, NULL);
	poRectShape *rect = new poRectShape(tex);
	addChild(rect);
}

void TestObj::eventHandler(poEvent *event) {
}
