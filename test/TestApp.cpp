#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"

void setupApplication() {
	poApplication *app = poApplication::get();
	app->addWindow(new poWindow(new TestObj(), WINDOW_TYPE_NORMAL, "MEH", 100, 100, 800, 600));
}

void cleanupApplication() {
}

void TestObj::setup() {
	glEnable(GL_TEXTURE_2D);
	FreeImage_Initialise();

	poEventCenter::get()->registerForEvent(PO_MOUSE_DOWN_EVENT, this);
	poEventCenter::get()->registerForEvent(PO_MOUSE_UP_EVENT, this);
	
	glClearColor(0,0,0,1);
	
	poShape2D* shape = new poShape2D();
	shape->addPoint(poPoint(0,0,0));
	shape->addPoint(poPoint(0,100,0));
	shape->addPoint(poPoint(100,100,0));
	shape->addPoint(poPoint(100,0,0));
	
	shape->placeTexture(poTexture(poImage("images/alfred_e_neuman.jpg")));
	shape->strokeWidth(30);
	shape->closed(true);
	shape->generateStroke();
	shape->position.set(100,300,0);
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
	static int pt_idx = -1;
	
	switch(event->type) {
		case PO_MOUSE_DOWN_EVENT:
		{
			pt_idx = -1;
			poShape2D *shape = getChildAs<poShape2D>(this,0);
			for(int i=0; i<shape->numPoints(); i++) {
				poPoint pt = shape->getPoint(i);
				if((pt-shape->globalToLocal(event->position)).length() < 10) {
					pt_idx = i;
					drag_event_id = poEventCenter::get()->registerForEvent(PO_MOUSE_MOVE_EVENT, this);
				}
			}
			break;
		}
			
		case PO_MOUSE_UP_EVENT:
			poEventCenter::get()->removeEvent(drag_event_id);
			break;
			
		case PO_MOUSE_MOVE_EVENT:
		{
			poShape2D *shape = getChildAs<poShape2D>(this,0);
			poPoint &pt = shape->getPoint(pt_idx);
			pt = shape->globalToLocal(event->position);
			pt.z = 0.f;
			shape->generateStroke();
			break;
		}
	}
}
