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
	
	poFont font("Helvetica", 50);
	poTextBox *tb = new poTextBox(300,300);
	tb->text("hello world button").font(PO_FONT_REGULAR, &font).buttonize(poColor::red, poColor::green, 50, 50).textAlignment(PO_ALIGN_CENTER_CENTER).layout().position(100,100);
	tb->addEvent(PO_MOUSE_PRESS_EVENT, this);
	tb->addEvent(PO_MOUSE_RELEASE_EVENT, this);
	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	poTextBox *tb = static_cast<poTextBox*>(event->source);
	if(event->type == PO_MOUSE_PRESS_EVENT)
		tb->buttonize(poColor::green, poColor::red, 50, 50);
	else
		tb->buttonize(poColor::red, poColor::green, 50, 50);
}


