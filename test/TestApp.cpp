#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
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

TestObj::TestObj() {
    addModifier(new poCamera2D());
	
	poObject *obj = new poObject();
	obj->position(100,100);
	addChild(obj);
	
	poRectShape *rect = new poRectShape(100,100);
	rect->setAlignment(PO_ALIGN_CENTER_CENTER);
	rect->fillColor(poColor::green);
	rect->rotation_tween.set(360).setRepeat(PO_TWEEN_REPEAT_REGULAR).setTweenFunction(linearFunc).setDuration(10.0).start();
	obj->addChild(rect);
	
	tb = new poTextBox(100,100);
//	tb->setAlignment(PO_ALIGN_CENTER_CENTER);
	obj->addChild(tb);
}

void TestObj::update() {
	tb->text( (boost::format("%d")%getWindowFramerate()).str() );
	tb->layout();
}

void TestObj::eventHandler(poEvent *event) {
}
