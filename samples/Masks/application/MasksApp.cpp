#include "MasksApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poShapeBasics2D.h"
#include "poResource.h"
#include "poMaskModifier.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new MasksApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 800, 800);
}

void cleanupApplication() {
	poCommon::get()->write("common.xml");
}

MasksApp::MasksApp() {
    addModifier(new poCamera2D(poColor::black,false));
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	holder = new poObject();
	holder->position(400,400,0);
	addChild(holder);
	
	setupAperture();
	start();
}

void MasksApp::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_KEY_DOWN_EVENT:
			if(event->modifiers & PO_KEY_META) {
				if(event->keyCode == 'o') {
					stop();
					open();
				}
				else if(event->keyCode == 'c') {
					close();
					start();
				}
			}
			break;
	}
}

void MasksApp::setupAperture() {
	// make the elements of the aperture
	int size = 200.f;
	float step = M_2PI / 12.f;
	for(int i=0; i<6; i++) {
		poShape2D *shape = new poShape2D();
		
		poPoint p1 = poPoint(0,0);
		poPoint p2 = poPoint(cosf(-step), sinf(-step)) * size;
		poPoint p3 = poPoint(cosf(step), sinf(step)) * size;
		
		shape->addPoint(p1);
		shape->addPoint(p2);
		shape->addPoint(p3);
		shape->calculateBounds();
		
		shape->fillColor(hashPointerForColor(shape))
		.fillDrawStyle(GL_TRIANGLES)
		.matrixOrder(PO_MATRIX_ORDER_RST)
		.rotation(rad2deg(i*step*2.f));
		holder->addChild(shape);
	}
	
	// make the masking object
	poOvalShape circle(size*1.7f, size*1.7f, 50);
	poGeometryMask *mask = new poGeometryMask(&circle);
	// and apply it
	holder->addModifier(mask);
}

void MasksApp::start() {
	holder->rotation_tween
	.set(holder->rotation() + 360)
	.setTweenFunction(linearFunc)
	.setDuration(5.f)
	.setRepeat(PO_TWEEN_REPEAT_REGULAR)
	.start();
}

void MasksApp::stop() {
	holder->rotation_tween.stop();
}

void MasksApp::open() {
	for(int i=0; i<holder->numChildren(); i++) {
		poShape2D *shape = getChildAs<poShape2D>(holder, i);
		shape->position_tween
		.set(poPoint(0,100,0))
		.setTweenFunction(quadInOutFunc)
		.setDuration(.2f)
		.start();
	}
}

void MasksApp::close() {
	for(int i=0; i<holder->numChildren(); i++) {
		poShape2D *shape = getChildAs<poShape2D>(holder, i);
		shape->position_tween
		.set(poPoint(0,0,0))
		.start();
	}
}

