#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"

void setupApplication() {
	poApplication *app = poApplication::get();
	app->addWindow(new poWindow(new TestObj(), WINDOW_TYPE_NORMAL, "MEH", 100, 100, 800, 600));
}

void cleanupApplication() {
}

void TestObj::setup() {
	poEventCenter::get()->registerForEvent(PO_MOUSE_DOWN_EVENT, this);
	poEventCenter::get()->registerForEvent(PO_MOUSE_UP_EVENT, this);
	
	glClearColor(0,0,0,1);
	
	poShape2D* shape = new poShape2D();
	shape->fillDrawStyle(GL_LINE_STRIP);
	shape->addPoint(poPoint(0,0,0));
	shape->addPoint(poPoint(0,150,0));
	shape->addPoint(poPoint(150,0,0));
	shape->addPoint(poPoint(150,150,0));
	
	shape->position.set(100,300,0);
	shape->strokeColor(poColor(1,0,1,1));
	shape->joinStyle(poShape2D::STROKE_JOIN_ROUND);
	shape->strokeWidth(25);
	addChild(shape);
}

void TestObj::update() {
}

void TestObj::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_DOWN_EVENT:
		{
			poShape2D *shape = getChildAs<poShape2D>(this,0);
			poPoint pt = shape->getPoint(2);
			if((pt-shape->globalToLocal(event->position)).length() < 10) {
				drag_event_id = poEventCenter::get()->registerForEvent(PO_MOUSE_MOVE_EVENT, this);
			}
			break;
		}
			
		case PO_MOUSE_UP_EVENT:
			poEventCenter::get()->removeEvent(drag_event_id);
			break;
			
		case PO_MOUSE_MOVE_EVENT:
		{
			poShape2D *shape = getChildAs<poShape2D>(this,0);
			poPoint &pt = shape->getPoint(2);
			pt = shape->globalToLocal(event->position);
			pt.z = 0.f;
			shape->enableStroke(true);
			break;
		}
	}
}
