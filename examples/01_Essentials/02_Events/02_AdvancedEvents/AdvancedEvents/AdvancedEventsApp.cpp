/////////////////////////////////////////
//
// pocode : Advanced Events
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
	
	Abottom = new poRectShape(100, 100);							// Draw a rectangle
	Abottom->fillColor = poColor::blue;
	Abottom->generateStroke(1);
	Abottom->strokeColor = poColor::black;
	Abottom->position.set(95, 180, 0);
	Abottom->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event A");	// Add a mouse down inside event to it
	addChild(Abottom);
	
	Atop = new poRectShape(100, 100);								// Draw a rectangle on top of the previous one
	Atop->fillColor = poColor::blue;
	Atop->generateStroke(1);
	Atop->strokeColor = poColor::black;
	Atop->position.set(115, 200, 0);
	Atop->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event A");	// Add a mouse down inside event to it
	addChild(Atop);
	
	
	// B. Visible / invisible objects ///////////////////////
	
	for(int i=0; i < 3; i++) {										// Draw 3 rectangles on top of each other
		
		float rectSize = 100 - (i * 10);							// Slightly reduce the size for each new rect
		
		poRectShape* Brect = new poRectShape(rectSize, rectSize);	// Create the rectangle
		Brect->fillColor = poColor::blue;
		Brect->generateStroke(1);
		Brect->strokeColor = poColor::black;
		Brect->setAlignment(PO_ALIGN_CENTER_CENTER);
		Brect->position.set(400, 240, 0);
		Brect->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "event B");
		addChild(Brect);
		
		poTextBox* Btext = new poTextBox(rectSize, rectSize);		// Draw a text on top of the rectangle
		Btext->setFont(poGetFont("Helvetica", "Bold"));
		Btext->setTextSize(35);
		Btext->textColor = poColor::white;
		Btext->setTextAlignment(PO_ALIGN_CENTER_CENTER);
		Btext->setAlignment(PO_ALIGN_CENTER_CENTER);
		
		if(i == 0) {												// Set the text to be a number from 1 to 3
			Btext->setText("3");									// The bottom rectangle is number 3
		}
		if(i == 1) {
			Btext->setText("2");									// The rectangle in the middle is number 2
		}
		if(i == 2) {
			Btext->setText("1");									// The top rectangle is number 1
		}
		
		Btext->doLayout();
		Brect->addChild(Btext);										// Add the text box as a child of the rectangle
		
		B.push_back(Brect);											// Save each rectangle into an array
	}
	
	
	// C. Alpha Test ///////////////////////
	
	img = new poImage("alphatest.png");								// Define a blue poImage for non-selected state
	img_over = new poImage("alphatest_over.png");					// Define a yellow poImage for selected state
	
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
	
	for(int i=0; i < B.size(); i++) {								// Check if one of the B rectangles is yellow
		
		if( B[i]->fillColor == poColor::yellow ) {					// If the rectangle is yellow,
																	// it means that the rect has been clicked on
			B[i]->alpha -= 0.01;									// Reduce the alpha value of the rectangle
			if( B[i]->alpha <= 0 ) {								// If the alpha value is 0,
				B[i]->visible = false;								// make the rectangle invisible
			}
		}
	}
}


// EVENT HANDLER
// Respond to user events here.
void AdvancedEventsApp::eventHandler(poEvent *event) {
	
	if( event->message == "event A" ) {						// Check if you receive a mouse press event
															// from one of the rectangles in A
		poRectShape* rect;									// Create a pointer to a poRectShape
		rect = (poRectShape*) event->source;				// Cast the event source to a poRectShape
		
		rect->fillColor = poColor::yellow;					// Make the rectangle yellow
	}
	else if( event->message == "event B" ) {				// Check if you receive a mouse press event
															// from one of the rectangles in B
		
		poRectShape* rect;									// Create a pointer to a poRectShape
		rect = (poRectShape*) event->source;				// Cast the event source to a poRectShape
		
		rect->fillColor = poColor::yellow;					// Make the rectangle yellow
	}
	else if( event->type == PO_MOUSE_ENTER_EVENT ) {		// Check if you receive a mouse enter event
															// from the transparent png.
		C->setImage(img_over);								// Load the image for mouse over state (yellow)
	}
	else if( event->type == PO_MOUSE_LEAVE_EVENT ) {		// Check if you receive a mouse leave event
															// from the transparent png.
		C->setImage(img);									// Load the image for default state (blue)
	}
	else if( event->type == PO_KEY_DOWN_EVENT ) {			// Verify if you get an event of type key down
		
		if( event->keyChar == 'r' ) {						// Verify which key was pressed
															// If the key is 'r' reset the color of each rectangle
			Abottom->fillColor = poColor::blue;
			Atop->fillColor = poColor::blue;
			
			for(int i=0; i < B.size(); i++) {
				
				B[i]->stopAllTweens();
				B[i]->visible = true;
				B[i]->alpha = 1.f;
				B[i]->fillColor = poColor::blue;
			}
		}
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AdvancedEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
