/////////////////////////////////////////
//
// poCode : Advanced Events
//
/////////////////////////////////////////

#include "AdvancedEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"


// APP CONSTRUCTOR.
// Create all objects here.
AdvancedEventsApp::AdvancedEventsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Events priority ///////////////////////
	
	Abottom = new poRectShape(100, 100);
	Abottom->fillColor = poColor::blue;
	Abottom->generateStroke(1);
	Abottom->strokeColor = poColor::black;
	Abottom->position.set(95, 180, 0);
	Abottom->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event A");
	addChild(Abottom);
	
	Atop = new poRectShape(100, 100);
	Atop->fillColor = poColor::blue;
	Atop->generateStroke(1);
	Atop->strokeColor = poColor::black;
	Atop->position.set(115, 200, 0);
	Atop->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event A");
	addChild(Atop);
	
	
	// B. Visible / invisible objects ///////////////////////
	
	for(int i=0; i < 3; i++) {
		
		float rectSize = 100;
		
		poRectShape* B;
		B = new poRectShape(rectSize, rectSize);
		B->fillColor = poColor::blue;
		B->generateStroke(1);
		B->strokeColor = poColor::black;
		B->setAlignment(PO_ALIGN_CENTER_CENTER);
		B->position.set(400, 240, 0);
		B->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event B");
		addChild(B);
		
		poTextBox* TB = new poTextBox(rectSize, rectSize);
		TB->setFont(poGetFont("Helvetica", "Bold"));
		TB->setTextSize(35);
		TB->textColor = poColor::white;
		TB->setTextAlignment(PO_ALIGN_CENTER_CENTER);
		TB->setAlignment(PO_ALIGN_CENTER_CENTER);
		B->addChild(TB);
		
		if(i == 0) { Bone = B; TB->setText("1"); }
		if(i == 1) { Btwo = B; TB->setText("2"); }
		if(i == 2) { Bthree = B;  TB->setText("3"); }
		
		TB->doLayout();
	}
	
	
	// C. Alpha Test ///////////////////////
	
	img = new poImage("alphatest.png");
	img_over = new poImage("alphatest_over.png");
	
	C = new poImageShape("alphatest.png", true);
	C->setAlphaTest(true);
	C->addEvent(PO_MOUSE_ENTER_EVENT, this);
	C->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	C->position.set(595, 190, 0);
	addChild(C);
	
	
	addEvent(PO_KEY_DOWN_EVENT, this);
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
	
	if( event->message == "event A" ) {
		
		if( event->source == Abottom ) {		
			Abottom->fillColor = poColor::yellow;
		}
		else if( event->source == Atop ) {		
			Atop->fillColor = poColor::yellow;
		}
	}
	else if( event->message == "event B" ) {
		
		poRectShape* rect;
		
		if( event->source == Bone ) {		
			rect = Bone;
		}
		else if( event->source == Btwo ) {		
			rect = Btwo;
		}
		else if( event->source == Bthree ) {		
			rect = Bthree;
		}
		
		rect->fillColor = poColor::yellow;
		
		rect->alpha -= 0.01;
		if(rect->alpha <= 0.99) rect->visible = false;
	}
	else if( event->type == PO_MOUSE_ENTER_EVENT ) {
		
		C->setImage(img_over);
	}
	else if( event->type == PO_MOUSE_LEAVE_EVENT ) {
		
		C->setImage(img);
	}
	else if( event->type == PO_KEY_DOWN_EVENT ) {
		
		if( event->keyChar == 'r' ) {
			
			Abottom->fillColor = poColor::blue;
			Atop->fillColor = poColor::blue;
			
			Bone->visible = true;
			Bone->alpha = 1.f;
			Bone->fillColor = poColor::blue;
			
			Btwo->visible = true;
			Btwo->alpha = 1.f;
			Btwo->fillColor = poColor::blue;
			
			Bthree->visible = true;
			Bthree->alpha = 1.f;
			Bthree->fillColor = poColor::blue;
		}
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AdvancedEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
