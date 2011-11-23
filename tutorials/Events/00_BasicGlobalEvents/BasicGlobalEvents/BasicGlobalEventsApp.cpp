/////////////////////////////////////////
//
// LESSON : Basic Global Events
//
/////////////////////////////////////////

#include "BasicGlobalEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR.
// Create all objects here.
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
	
	addEvent(PO_MOUSE_DOWN_EVENT, this);				// PO_MOUSE_DOWN is global.
														// You can click anywhere in the window
														// BasicEventsApp (this) is both sender and receiver
	// Draw a shape that will change color
	A = new poRectShape(75, 75);
	A->fillColor.set(1, 0.85, 0);
	A->position.set(90, 264, 0);
	addChild(A);
	
	
	// B. PO_MOUSE_UP_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_UP_EVENT, this);
	
	// Draw a shape that will change color
	B = new poRectShape(75, 75);
	B->fillColor.set(1, 0.85, 0);
	B->position.set(275, 264, 0);
	addChild(B);
	
	
	// C. PO_MOUSE_DRAG_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	// Draw a shape that will change color
	C = new poRectShape(75, 75);
	C->fillColor.set(1, 0.85, 0);
	C->position.set(455, 264, 0);
	addChild(C);
	
	
	// D. PO_KEY_DOWN_EVENT ///////////////////////
	
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	// Draw a shape that will change color
	D = new poRectShape(75, 75);
	D->fillColor.set(1, 0.85, 0);
	D->position.set(640, 264, 0);
	addChild(D);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
BasicGlobalEventsApp::~BasicGlobalEventsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void BasicGlobalEventsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void BasicGlobalEventsApp::eventHandler(poEvent *event) {
	
	// Define actions based on the features of poEvent (type, name, message, etc...)
	
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
			A->fillColor.set(1, 0.85, 0);
			B->fillColor.set(1, 0.85, 0);
			C->fillColor.set(1, 0.85, 0);
			D->fillColor.set(1, 0.85, 0);
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


// MESSAGE HANDLER
// Receive inter-object messages here.
void BasicGlobalEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
