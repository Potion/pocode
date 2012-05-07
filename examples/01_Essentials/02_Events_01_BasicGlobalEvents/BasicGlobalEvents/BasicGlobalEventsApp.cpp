/////////////////////////////////////////
//
// pocode : Basic Global Events
//
/////////////////////////////////////////

#include "BasicGlobalEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes


// APP CONSTRUCTOR. Create all objects here.
BasicGlobalEventsApp::BasicGlobalEventsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A NOTE ON EVENTS ///////////////////////
	// Every poObject has a addEvent() function
	// Each event that occurs has a sender and a receiver of the poEvent
	// Sender->addEvent(eventType, Receiver)
	// The receiver will receive the poEvent in the eventHandler()
	
	
	// A. PO_MOUSE_DOWN_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_DOWN_EVENT, this);			// PO_MOUSE_DOWN is global.
													// You can click anywhere in the window
													// BasicEventsApp (this) is both sender and receiver
	
	A = new poRectShape(75, 75);					// Draw a shape that will change color
	A->fillColor.set(1, 0.85, 0);
	A->position.set(90, 204, 0);
	addChild(A);
	
	
	// B. PO_MOUSE_UP_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_UP_EVENT, this);				// Add a "mouse up" event
	
	B = new poRectShape(75, 75);					// Draw a shape that will change color
	B->fillColor.set(1, 0.85, 0);
	B->position.set(270, 204, 0);
	addChild(B);
	
	
	// C. PO_MOUSE_DRAG_EVENT ///////////////////////
	
	addEvent(PO_MOUSE_DRAG_EVENT, this);			// Add a "mouse drag" event
	
	C = new poRectShape(75, 75);					// Draw a shape that will change color
	C->fillColor.set(1, 0.85, 0);
	C->position.set(455, 204, 0);
	addChild(C);
	
	
	// D. PO_KEY_DOWN_EVENT ///////////////////////
	
	addEvent(PO_KEY_DOWN_EVENT, this);				// Add a "key down" event
	
	D = new poRectShape(75, 75);					// Draw a shape that will change color
	D->fillColor.set(1, 0.85, 0);
	D->position.set(640, 204, 0);
	addChild(D);
}


// APP DESTRUCTOR. Delete all objects here.
BasicGlobalEventsApp::~BasicGlobalEventsApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void BasicGlobalEventsApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void BasicGlobalEventsApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void BasicGlobalEventsApp::eventHandler(poEvent *event) {
	
	// Define actions based on the features of poEvent (type, name, message, etc...)
	
	if(event->type == PO_MOUSE_DOWN_EVENT) {		// Verify if you receive an event of type mouse down
		
		A->fillColor = poColor::blue;				// Change the color of rectangle A
	}
	else if(event->type == PO_MOUSE_UP_EVENT) {		// Verify if you receive an event of type mouse up		
		
		B->fillColor = poColor::blue;				// Change the color of rectangle B
	}
	else if(event->type == PO_MOUSE_DRAG_EVENT) {	// Verify if you receive an event of type mouse drag
		
		C->fillColor = poColor::blue;				// Change the color of rectangle C
	}
	else if(event->type == PO_KEY_DOWN_EVENT) {		// Verify if you receive an event of type key down
		
		if(event->keyChar == 'r') {					// Verify which key was pressed
													// If the key is 'r' reset the color of each rectangle
			A->fillColor.set(1, 0.85, 0);
			B->fillColor.set(1, 0.85, 0);
			C->fillColor.set(1, 0.85, 0);
			D->fillColor.set(1, 0.85, 0);
		}
		else {										// Else, if it's any other key
			
			D->fillColor = poColor::blue;			// Change the color of rectangle D
		}
	}
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void BasicGlobalEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
