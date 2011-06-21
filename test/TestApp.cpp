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
	addEvent(PO_WINDOW_RESIZED_EVENT, this);
	
	poShape2D *img = new poRectShape("images/testimg.png");
	img->alignment(PO_ALIGN_CENTER_CENTER);
	img->rotation_tween.set(360).setDuration(10).setRepeat(PO_TWEEN_REPEAT_REGULAR).setTweenFunction(linearFunc).start();
	img->alphaTestTextures(true);
	img->addEvent(PO_MOUSE_ENTER_EVENT, this);
	img->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	addChild(img);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_WINDOW_RESIZED_EVENT:
			event->source->getChild(0)->position(getWindowWidth()/2, getWindowHeight()/2);
			break;
			
		case PO_MOUSE_ENTER_EVENT:
			printf("enter\n");
			break;
			
		case PO_MOUSE_LEAVE_EVENT:
			printf("leave\n");
			break;
	}
}


