#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;
	
poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 500, 500);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
//	poShape2D *shape = new poRectShape(150,150);
//	shape->alignment(PO_ALIGN_CENTER_CENTER);
//	shape->position(getWindowWidth()/2.f, getWindowHeight()/2.f);
//	addChild(shape);
//	
//	poGUIPointer *pointer = new poGUIPointer("position", poPoint(), poRect(-100,-100,200,200), boost::bind(&poObject::position, shape, _1));
//	addChild(pointer);
	
	poTextBox *tb = new poTextBox(200,200);
	tb->text("hello world").font(PO_FONT_REGULAR, poFont("Helvetica",25)).layout();
	tb->alpha_tween.set(0).setRepeat(PO_TWEEN_REPEAT_REGULAR).start();
	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}


