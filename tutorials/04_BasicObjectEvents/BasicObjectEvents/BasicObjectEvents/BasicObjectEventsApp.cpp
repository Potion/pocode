/////////////////////////////////////////
//
// LESSON : Basic Object Events
//
/////////////////////////////////////////

#include "BasicObjectEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BasicObjectEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicObjectEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}

BasicObjectEventsApp::BasicObjectEventsApp() {
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// NOTES ON EVENTS AND GLOBAL EVENTS ///////////////////////
	
	// Every poObject has a addEvent() function
	// Each event that occurs has a sender and a receiver of the poEvent
	// Sender->addEvent(eventType, Receiver)
	
	
	// A. PO_MOUSE_DOWN_INSIDE_EVENT ///////////////////////
	
	A = new poRectShape(75, 75);
	A->fillColor = poColor::yellow;
	A->generateStroke(1);
	A->strokeColor = poColor::black;
	A->position.set(90, 264, 0);
	addChild(A);
	
	A->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);				// Add the event to the shape
	
	
	// B. PO_MOUSE_ENTER_EVENT ///////////////////////
	
	B = new poRectShape(75, 75);
	B->fillColor = poColor::yellow;
	B->generateStroke(1);
	B->strokeColor = poColor::black;
	B->position.set(275, 264, 0);
	addChild(B);
	
	B->addEvent(PO_MOUSE_ENTER_EVENT, this);
	
	
	// C. PO_MOUSE_LEAVE_EVENT ///////////////////////
	
	C = new poRectShape(75, 75);
	C->fillColor = poColor::yellow;
	C->generateStroke(1);
	C->strokeColor = poColor::black;
	C->position.set(455, 264, 0);
	addChild(C);
	
	C->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	
	
	// D. PO_MOUSE_OVER_EVENT ///////////////////////
	
	D = new poRectShape(75, 75);
	D->fillColor = poColor::yellow;
	D->generateStroke(1);
	D->strokeColor = poColor::black;
	D->position.set(640, 264, 0);
	addChild(D);
	
//	D->addEvent(PO_MOUSE_ENTER_EVENT, this);	// INTERNAL!!! we need ENTER for OVER to work. Is that ok?
	D->addEvent(PO_MOUSE_OVER_EVENT, this);
	
	
	// Key event to reset the color
	addEvent(PO_KEY_DOWN_EVENT, this);
}

BasicObjectEventsApp::~BasicObjectEventsApp() {
}

void BasicObjectEventsApp::eventHandler(poEvent *event) {
	
	if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {
		
		A->fillColor = poColor::blue;
	}
	else if(event->type == PO_MOUSE_ENTER_EVENT) {
		
		B->fillColor = poColor::blue;
	}
	else if(event->type == PO_MOUSE_LEAVE_EVENT) {
		
		C->fillColor = poColor::blue;
	}
	else if(event->type == PO_MOUSE_OVER_EVENT) {
		
		D->fillColor = poColor::blue;
		printf("OVER\n");
	}
	else if(event->type == PO_KEY_DOWN_EVENT) {
		
		if(event->keyChar == 'r') {						// Verify which key was pressed
			A->fillColor = poColor::yellow;
			B->fillColor = poColor::yellow;
			C->fillColor = poColor::yellow;
			D->fillColor = poColor::yellow;
		}
	}
}

void BasicObjectEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
