/////////////////////////////////////////
//
// poCode : Basic Object Events
//
/////////////////////////////////////////

#include "AdvancedEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR.
// Create all objects here.
AdvancedEventsApp::AdvancedEventsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Visible / invisible objects ///////////////////////
	
	for(int i=0; i < 3; i++) {
		
		poOvalShape* A = new poOvalShape(100 - (i * 10), 100 - (i * 10), 50);
		A->fillColor = poColor::blue;
		A->generateStroke(1);
		A->strokeColor = poColor::black;
		A->position.set(160, 240, 0);
		A->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
		addChild(A);
	}
	
	
	// B. Alpha Test ///////////////////////
	
	img = new poTexture("alphatest.png");
	img_over = new poTexture("alphatest_over.png");
	
	B = new poRectShape(img);
	B->addEvent(PO_MOUSE_ENTER_EVENT, this);
	B->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	B->position.set(350, 190, 0);
	addChild(B);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
AdvancedEventsApp::~AdvancedEventsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void AdvancedEventsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void AdvancedEventsApp::eventHandler(poEvent *event) {
	
	if( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
		
		poShape2D* shape = (poShape2D*) event->source;
		shape->fillColor = poColor::yellow;
		
		if(shape->alpha <= 0) shape->visible = false;
		shape->alpha -= 0.15;
	}
	else if( event->type == PO_MOUSE_ENTER_EVENT ) {
		
		B->placeTexture(img_over);
	}
	else if( event->type == PO_MOUSE_LEAVE_EVENT ) {
		
		B->placeTexture(img);
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AdvancedEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
