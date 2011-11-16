/////////////////////////////////////////
//
// LESSON : Basic Global Events
//
/////////////////////////////////////////

#include "BasicGlobalEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BasicGlobalEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicGlobalEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}

BasicGlobalEventsApp::BasicGlobalEventsApp() {
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// NOTES ON EVENTS AND GLOBAL EVENTS ///////////////////////
	
	// every poObject has a addEvent() function
	// Each event that occurs has a sender and a receiver of the poEvent
	// Sender->addEvent(eventType, Receiver)
	
	
	// A. PO_MOUSE_DOWN_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_DOWN_EVENT, this);		// PO_MOUSE_DOWN is global because you can click anywhere in the window
	// In this case BasicEventsApp is both the sender and the receiver
	// The event will be handled in the eventHandler() function
	
	// Draw a shape that will change color
	A = new poRectShape(75, 75);
	A->fillColor = poColor::yellow;
	A->generateStroke(1);
	A->strokeColor = poColor::black;
	A->position.set(90, 264, 0);
	addChild(A);
	
	
	// B. PO_MOUSE_UP_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_UP_EVENT, this);
	
	// Draw a shape that will change color
	B = new poRectShape(75, 75);
	B->fillColor = poColor::yellow;
	B->generateStroke(1);
	B->strokeColor = poColor::black;
	B->position.set(275, 264, 0);
	addChild(B);
	
	
	// C. PO_MOUSE_DRAG_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	// Draw a shape that will change color
	C = new poRectShape(75, 75);
	C->fillColor = poColor::yellow;
	C->generateStroke(1);
	C->strokeColor = poColor::black;
	C->position.set(455, 264, 0);
	addChild(C);
	
	
	// D. PO_KEY_DOWN_EVENT ///////////////////////
	
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	// Draw a shape that will change color
	D = new poRectShape(75, 75);
	D->fillColor = poColor::yellow;
	D->generateStroke(1);
	D->strokeColor = poColor::black;
	D->position.set(640, 264, 0);
	addChild(D);
}

BasicGlobalEventsApp::~BasicGlobalEventsApp() {
}

// The receiver poObject receives the poEvent through the eventHandler() function

void BasicGlobalEventsApp::eventHandler(poEvent *event) {
	
	// Define actions based on the features of the poEvent (type, name, message, etc...)
	
	if(event->type == PO_MOUSE_DOWN_EVENT) {			// Verify the event type
		
		A->fillColor = poColor::blue;
		printf("PO_MOUSE_DOWN_EVENT\n");
	}
	else if(event->type == PO_MOUSE_UP_EVENT) {
		
		B->fillColor = poColor::blue;
		printf("PO_MOUSE_UP_EVENT\n");
	}
	else if(event->type == PO_MOUSE_DRAG_EVENT) {
		
		C->fillColor = poColor::blue;
		printf("PO_MOUSE_DRAG_EVENT\n");
	}
	else if(event->type == PO_KEY_DOWN_EVENT) {
		
		if(event->keyChar == 'r') {						// Verify which key was pressed
			A->fillColor = poColor::yellow;
			B->fillColor = poColor::yellow;
			C->fillColor = poColor::yellow;
			D->fillColor = poColor::yellow;
		}
		else {
			D->fillColor = poColor::blue;
		}
		printf("PO_KEY_DOWN_EVENT\n");
	}
	else if(event->type == PO_KEY_UP_EVENT) {
		
		D->fillColor = poColor::yellow;
		printf("PO_KEY_UP_EVENT\n");
	}
}

void BasicGlobalEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
