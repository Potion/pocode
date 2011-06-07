#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 800, 800);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier((new poPerspectiveCamera(65, 4.f/3.f, 0.1, 100.f))->lookAt(poPoint(0,0,0)));
	position(0,0,100);
	
	poTextBox *tb = new poTextBox(100,100);
	tb->text("hello world");
	tb->layout();
	addChild(tb);
}

void TestObj::eventHandler(poEvent *event) {
}
