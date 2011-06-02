#include "MasksApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poShapeBasics2D.h"
#include "poResource.h"
#include "poMaskModifier.h"

using namespace std;

// make root objects for each of our windows
poObject *createObjectForID(uint uid) {
	return new MasksApp();
}

// create however many windows we need
void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 800, 800);
}

// clean up the application before it closes
void cleanupApplication() {
	poCommon::get()->write("common.xml");
}

// do the application setup here
MasksApp::MasksApp() {
	// this 2D camera will also clear the background
    addModifier(new poCamera2D(poColor::black));
	// register to receive key events
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	// make a holder for the leaves of the aperture
	holder = new poObject();
	holder->position(400,400,0);
	addChild(holder);
	
	// setup the aperture
	setupAperture();
	
	// and start the animation
	start();
}

// all events targeted at the MasksApp object come thru here
void MasksApp::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_KEY_DOWN_EVENT:
			// check if the cmd key is down
			if(event->modifiers & PO_KEY_META) {
				// key code is a numeric representation, without modifiers
				// e.g. 'shift-t' would still be 't', not 'T'
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
		
		// you have to manually call calculateBounds
		shape->calculateBounds();
		
		// fill out the parameter for this shape
		shape->fillColor(hashPointerForColor(shape))
			.fillDrawStyle(GL_TRIANGLES)
			// change the order in which transformations get applied
			.matrixOrder(PO_MATRIX_ORDER_RST)
			// this is in degrees
			.rotation(rad2deg(i*step*2.f));
		
		// store the triangle in the holder
		holder->addChild(shape);
	}
	
	// make the masking object from a poShape2D
	poOvalShape circle(size*1.7f, size*1.7f, 50);
	poGeometryMask *mask = new poGeometryMask(&circle);
	// and apply it
	holder->addModifier(mask);
}

void MasksApp::start() {
	// every poObject has a batch of tweens built-in
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

