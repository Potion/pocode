#include "BasicEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BasicEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}

BasicEventsApp::BasicEventsApp() {
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	float width = getWindowWidth();
	float height = getWindowHeight();
	
	rectangle = new poRectShape(150, 150);
	rectangle->fillColor = poColor::dkGrey;
	rectangle->alpha = 0.5;
	rectangle->position.set(width/2,280,0);
	rectangle->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(rectangle);
	
	
	// Add Events
	
	// poObjects have a addEvent() function
	// Each event that occurs has a sender and a receiver of the poEvent
	// Sender->addEvent(eventType, Receiver)
	
	// Add a key event
	addEvent(PO_KEY_DOWN_EVENT, this);				// BasicEventsApp is both the sender and the receiver
	
	// Add mouse generic events
	addEvent(PO_MOUSE_DOWN_EVENT, this);			// These mouse events can be added to any poObject, not just poShapes
	addEvent(PO_MOUSE_UP_EVENT, this);				// Wherever you click, the event is received
	
	// Shape-dependent events
	// These events happen only within the bounds of a poShape
	rectangle->addEvent(PO_MOUSE_ENTER_EVENT, this);
	rectangle->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	rectangle->addEvent(PO_MOUSE_OVER_EVENT, this);
    rectangle->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
}

BasicEventsApp::~BasicEventsApp() {
}

// The receiver poObject receives the poEvent through the eventHandler() function
void BasicEventsApp::eventHandler(poEvent *event) {
	
	// Define actions based on the features of the poEvent (type, name, message, etc...)
	
	// Check the event type
	if(event->type == PO_KEY_DOWN_EVENT)
	{
		// Check information on which key was pressed
		// event->keyChar tells you which character was pressed
		if(event->keyChar == ' ') {
			printf("YOU HIT THE SPACE BAR!\n");
		}
		// event->keyCode, if you want to check for special keys (SHIFT, RETURN, ARROWS, F1, F2 etc...)
		else if(event->keyCode == PO_RETURN_KEY) {
			printf("YOU HIT ENTER!\n");
		}
		else {
			printf("PO_KEY_DOWN_EVENT\n");
		}
	}
	else if(event->type == PO_MOUSE_DOWN_EVENT) {
		
		printf("PO_MOUSE_DOWN_EVENT\n");
	}
	else if(event->type == PO_MOUSE_UP_EVENT) {
		
		rectangle->scale.set(1,1,1);				// set the scale to 1.0 when you click over the rectangle
		
		printf("PO_MOUSE_UP_EVENT\n");
	}
	else if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {
		
		rectangle->scale.set(0.9,0.9,1);			// set the scale to 0.9 when you click over the rectangle
		
		printf("PO_MOUSE_DOWN_INSIDE_EVENT\n");
	}
	else if(event->type == PO_MOUSE_ENTER_EVENT) {
		
		// event->source returns a poObject pointer to the sender object (rectangle)
		event->source->alpha = 1.0;					// set the alpha to 1.0 when you enter
		
		printf("PO_MOUSE_ENTER_EVENT\n");
	}
	else if(event->type == PO_MOUSE_LEAVE_EVENT) {
		
		event->source->alpha = 0.5;					// set the alpha back to 0.5 when you leave
		
		printf("PO_MOUSE_LEAVE_EVENT\n");
	}
	else if(event->type == PO_MOUSE_OVER_EVENT) {
		
		printf("PO_MOUSE_OVER_EVENT\n");
	}
}

void BasicEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
