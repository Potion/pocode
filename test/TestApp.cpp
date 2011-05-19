#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"
#include "poTextBox.h"

#define TEST_OBJ_ID 1000

poObject *createObjectForID(uint uid) {
	switch(uid) {
		case TEST_OBJ_ID:
			return new TestObj();
			break;
	}
	
	return NULL;
}

void setupApplication() {
	applicationCreateWindow(TEST_OBJ_ID, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 800, 600);
	applicationCreateWindow(TEST_OBJ_ID, WINDOW_TYPE_NORMAL, "MainWindow 2", 200, 200, 800, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	this->alpha = 0.f;
	this->alpha_tween
		.set(1.f)
		.setDuration(2.f)
		.setRepeat(PO_TWEEN_REPEAT_PINGPONG)
		.setTweenFunction(expoInOutFunc)
		.start();
	
	poImage *img = resources.add(new poImage("images/alfred_e_neuman.jpg"));
	poTexture *tex = resources.add(new poTexture(img));
	
	poShape2D *shape = new poShape2D();
	shape->addPoint(poPoint(0,0))
		.addPoint(poPoint(tex->width(), 0))
		.addPoint(poPoint(tex->width(), tex->height()))
		.addPoint(poPoint(0, tex->height()));
	
	shape->placeTexture(tex);
	addChild(shape);

	glClearColor(0,0,0,1);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TestObj::preDraw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TestObj::eventHandler(poEvent *event) {
}
