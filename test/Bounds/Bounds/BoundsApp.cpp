#include "BoundsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BoundsApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Bounds", 100, 100, 600, 300);
}

void cleanupApplication() {
}

BoundsApp::BoundsApp() {
	addModifier(new poCamera2D(poColor::black));
	
	pop = new poObject();
	addChild(pop);
	
	kid1 = new poRectShape( 100.f, 100.f );
	kid1->position = poPoint( 100.f, 50.f );
	pop->addChild(kid1);

	
	kid2 = new poRectShape( 100.f, 100.f );
	kid2->position = poPoint( 400.f, 150.f );
	pop->addChild(kid2);
	
	kid1->addEvent(PO_MOUSE_DRAG_EVENT, this);
	kid2->addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

BoundsApp::~BoundsApp() {
}

void BoundsApp::draw() {
	
}

void BoundsApp::eventHandler(poEvent *event) {
	
	if( event->type == PO_MOUSE_DRAG_EVENT ){
	
		event->source->position = event->position;
	}
	
	else if( event->type == PO_KEY_DOWN_EVENT ){

		pop->bounds = pop->calculateBounds();
		std::cout<<"the bounds for pop are "<<pop->bounds<<"\n";
	
	}
}

void BoundsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
