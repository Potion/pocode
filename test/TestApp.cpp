#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"
#include "poResource.h"
#include "poMaskModifier.h"

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
	// make sure whatever global props you changed get written out
	poCommon::get()->write("common.xml");
}

TestObj::TestObj() {
    addModifier(new poCamera2D(poColor::black));
	
	addChild(new poRectShape(500,500));
	
	poResourceStore tmp;
	poTexture *tex = tmp.add(new poTexture("images/meatballspoon.jpg"));

	poRectShape *rect = new poRectShape(tex);
//	rect->addModifier(new poImageMask("images/mask.png"));
	rect->addModifier(new poGeometryMask(poRect(10,10,200,200)));
	rect->addEvent(PO_MOUSE_PRESS_EVENT, this);
	rect->addEvent(PO_MOUSE_DRAG_EVENT, this);
	rect->drawBounds(true);
	addChild(rect);
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_PRESS_EVENT) {
		event->source->offset(event->local_position);
		event->source->position(event->position);
	}
	else if(event->type == PO_MOUSE_DRAG_EVENT) {
		event->source->position(event->position);
	}
}


