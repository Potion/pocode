/////////////////////////////////////////
//
// LESSON : Basic Object Events
//
/////////////////////////////////////////

#include "BasicObjectEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR.
// Create all objects here.
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
	A->fillColor.set(1, 0.85, 0);
	A->position.set(90, 204, 0);
	addChild(A);
	
	A->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);				// Add the event to the shape
	
	
	// B. PO_MOUSE_ENTER_EVENT ///////////////////////
	
	B = new poRectShape(75, 75);
	B->fillColor.set(1, 0.85, 0);
	B->position.set(270, 204, 0);
	addChild(B);
	
	B->addEvent(PO_MOUSE_ENTER_EVENT, this);
	
	
	// C. PO_MOUSE_LEAVE_EVENT ///////////////////////
	
	C = new poRectShape(75, 75);
	C->fillColor.set(1, 0.85, 0);
	C->position.set(455, 204, 0);
	addChild(C);
	
	C->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	
	
	// D. PO_MOUSE_OVER_EVENT ///////////////////////
	
	D = new poRectShape(75, 75);
	D->fillColor.set(1, 0.85, 0);
	D->position.set(640, 204, 0);
	addChild(D);
	
//	D->addEvent(PO_MOUSE_ENTER_EVENT, this);	// INTERNAL!!! we need ENTER for OVER to work.
	D->addEvent(PO_MOUSE_OVER_EVENT, this);
	
	
	// Key event to reset the color
	addEvent(PO_KEY_DOWN_EVENT, this);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
BasicObjectEventsApp::~BasicObjectEventsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void BasicObjectEventsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
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
			A->fillColor.set(1, 0.85, 0);
			B->fillColor.set(1, 0.85, 0);
			C->fillColor.set(1, 0.85, 0);
			D->fillColor.set(1, 0.85, 0);
		}
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void BasicObjectEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
