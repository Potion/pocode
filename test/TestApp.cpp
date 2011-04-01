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
	glClearColor(0,0,0,1);
	
	poShape2D* shape = new poShape2D();
	shape->curveTo(poPoint(100,100,0), poPoint(100,0,0), 20);
	shape->curveTo(poPoint(0,0,0), poPoint(0,100,0), 20);
	shape->fillColor.set(1,0,0);
	shape->fillDrawStyle = GL_POLYGON;
	shape->strokeWidth = 10;
	shape->strokeColor.set(0,0,1);
	shape->setAlignment(PO_ALIGN_CENTER_CENTER);
	shape->position.set(100,100,0);
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

	drawRect(mouse.x-3, mouse.x+3, mouse.y-3, mouse.y+3);
}

bool TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_MOVE_EVENT) {
		mouse.set(event->localX, event->localY, 0);
	}
	return false;
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
	
}
