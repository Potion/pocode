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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 1050, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	poTextBox *tb = new poTextBox(200,200);
	poResourceStore tmp;
	tb->text("hello world VA").font("", tmp.add(new poFont("Lucida Grande", 10))).position(100,100);
	tb->layout();
	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT && isArrowKey(event->modifiers)) {
		poTextBox *tb = getChildAs<poTextBox>(this, 0);
		float pt = tb->font("")->pointSize();

		if(event->keyCode == PO_DOWN_ARROW) {
			pt = max(pt-1.f, 10.f);
		}
		else if(event->keyCode == PO_UP_ARROW) {
			pt = min(pt+1.f, 100.f);
		}

		poResourceStore tmp;
		tb->font("", tmp.add(new poFont("Lucida Grande", pt)));
		tb->layout();
	}
}
