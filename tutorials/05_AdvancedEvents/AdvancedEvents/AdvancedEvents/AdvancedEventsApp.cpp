#include "AdvancedEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}

AdvancedEventsApp::AdvancedEventsApp() {
	addModifier(new poCamera2D(poColor::black));
	
	poRectShape* BG = new poRectShape("bg.jpg");
	addChild(BG);
	
	float width = getWindowWidth();
	float height = getWindowHeight();
	
	// Draw some shapes
	
	redRect = new poRectShape(150, 150);
	redRect->name = "redRect";
	redRect->fillColor = poColor::red;
	redRect->alpha = 0.5;
	redRect->position.set(125,180,0);
	addChild(redRect);
	
	greenRect = new poRectShape(150, 150);
	greenRect->fillColor = poColor::green;
	greenRect->generateStroke(5);
	greenRect->strokeColor = poColor::yellow;
	greenRect->strokeEnabled = false;
	greenRect->position.set(325,180,0);
	addChild(greenRect);
	
	blueRect = new poRectShape(150, 150);
	blueRect->fillColor = poColor::blue;
	blueRect->generateStroke(5);
	blueRect->strokeColor = poColor::yellow;
	blueRect->strokeEnabled = false;
	blueRect->position.set(525,180,0);
	addChild(blueRect);
	
	hsvLine = new poLineShape(poPoint(0,0,0), poPoint(width-250,0,0));
	hsvLine->strokeColor = poHSVColor(0, 1, 1);
	hsvLine->position.set(125,380,0);
	addChild(hsvLine);
	
	// Add Events
	
	addEvent(PO_MOUSE_DOWN_EVENT, this);
	addEvent(PO_MOUSE_UP_EVENT, this);
	addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	// Make redRect react to mouse enter and leave
	redRect->addEvent(PO_MOUSE_ENTER_EVENT, this);
	redRect->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	
	// Make greenRect react to mouse press
	greenRect->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	
	// Make blueRect react to mouse drag
	blueRect->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	blueRect->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
	
	dragOffset.set(0,0,0);
}

AdvancedEventsApp::~AdvancedEventsApp() {
}

void AdvancedEventsApp::eventHandler(poEvent *event) {
	
	if(event->type == PO_MOUSE_DOWN_EVENT) {
		
		printf("PO_MOUSE_DOWN_EVENT\n");
	}
	else if(event->type == PO_MOUSE_UP_EVENT) {
		greenRect->strokeEnabled = false;
		blueRect->strokeEnabled = false;
		
		printf("PO_MOUSE_UP_EVENT\n");
	}
	else if(event->type == PO_MOUSE_DRAG_EVENT) {
		
		poHSVColor* C = new poHSVColor(hsvLine->strokeColor);
		C->H += .01;
		if(C->H > 1) C->H = 0;
		hsvLine->strokeColor.setHSV(C->H, C->S, C->V);
		printf("PO_MOUSE_DRAG_EVENT\n");
	}
	else if(event->type == PO_MOUSE_ENTER_EVENT) {
		
		// event->source returns a pointer to the poObject that registered the event
		// it has all the properties of poObjects, like alpha, position, etc...
		event->source->alpha = 1;
		
		printf("PO_MOUSE_ENTER_EVENT\n");
	}
	else if(event->type == PO_MOUSE_LEAVE_EVENT) {
		event->source->alpha = 0.5;
		
		printf("PO_MOUSE_LEAVE_EVENT\n");
	}
	else if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {
		
		// Since we know that this event comes from a poRectShape
		// we can treat event->source as a poShape2D by casting it
		poShape2D* shape = (poShape2D*) event->source;
		shape->strokeEnabled = true;
		
		if(event->source == blueRect){
			dragOffset = event->localPosition;
		}
		printf("Event: PO_MOUSE_DOWN_INSIDE_EVENT\n");
	}
	else if(event->type == PO_MOUSE_DRAG_INSIDE_EVENT) {
		event->source->alpha = 1;
		
//		event->source->position = event->globalPosition - dragOffset;
		event->source->position.x = event->globalPosition.x - dragOffset.x;
		event->source->position.y = event->globalPosition.y - dragOffset.y;
		
		printf("Event: PO_MOUSE_DRAG_INSIDE_EVENT\n");
	}
}

void AdvancedEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
