#include "MasksApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poShapeBasics2D.h"
#include "poResource.h"
#include "poMaskModifier.h"
//#include "poTextBox.h"

using namespace std;

// make root objects for each of our windows
poObject *createObjectForID(uint uid) {
	return new MasksApp();
}

// create however many windows we need
void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 600, 300);
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
	
	setupSurprise();
	setupAperture();
	setupInstructions();
	
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

void MasksApp::setupSurprise() {
	// load an image
	poRectShape *img = new poRectShape("PandaBaby9911.jpeg");
	addChild(img);
	// add an image-based mask to the thing
	img->addModifier(new poImageMask("mask.jpg"));
	
	// check the settings dictionary for where to put the thing
	poCommon *common = poCommon::get();
	std::string key = "panda position";
	
	poPoint location(0,0);
	// if the settings has it
	if(common->has(key))
		// move the panda to the right spot
		img->position(common->getPoint(key));
}

void MasksApp::setupAperture() {
	// make a holder for the leaves of the aperture
	holder = new poObject();
	holder->position(getWindowWidth()/2,getWindowHeight()/2,0);
	addChild(holder);
	
	// make the elements of the aperture
	int size = 150.f;
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

void MasksApp::setupInstructions() {
	/*
     poTextBox *text = new poTextBox(200,200);

	poFont reg("LucidaGrande", FONT_REGULAR, 20);
	poFont bol("LucidaGrande", FONT_BOLD, 20);
	text->fontMap()
		.font(poFontMap::REGULAR_FONT_KEY, &reg)
		.font(poFontMap::BOLD_FONT_KEY, &bol);
	text->text("<b>⌘-o</b> to open\n"
			   "<b>⌘-c</b> to close\n");
	text->layout();
	addChild(text);
     */
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

