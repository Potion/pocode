/////////////////////////////////////////
//
// poCode : Dictionaries
//
/////////////////////////////////////////

#include "DictionariesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"


// APP CONSTRUCTOR.
// Create all objects here.
DictionariesApp::DictionariesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. Create a poDictionary and get data from it ///////////////////////
	
	poDictionary A;
	A.set("textSize", 25);
	A.set("text", "This text and its properties are stored in the poDictionary A.");
	A.set("color", poColor::orange);
	A.set("position", poPoint(60, 180));
	
	poTextBox* text = new poTextBox(300, 130);
	text->setFont( poGetFont("Helvetica", "Regular") );
	text->setText( A.getString("text") );
	text->textColor = A.getColor("color");
	text->setTextSize( A.getInt("textSize") );
	text->doLayout();
	text->position = A.getPoint("position");
	addChild(text);
	
	
	// B. Use of poDictionary in events ///////////////////////
	
	poRandSeed();
	
	for(int i=1; i < 9; i++) {
		
		poRectShape* rect = new poRectShape(30,30);
		rect->fillColor = poColor::random();
		rect->position.set(388 + (i * 40), 280, 0);
		addChild(rect);
		
		poDictionary B;
		B.set("rectID", i);
		B.set("rectColor", rect->fillColor);
		
		rect->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "rect clicked", B);
	}
	
	bigRect = new poRectShape(310, 90);
	bigRect->fillColor = poColor::ltGrey;
	bigRect->position.set(428, 180, 0);
	addChild(bigRect);
	
	rectTextBox = new poTextBox(310, 90);
	rectTextBox->setFont( poGetFont("Helvetica", "Regular") );
	rectTextBox->setText("Click on the colored rectangles to change color");
	rectTextBox->setTextSize(18);
	rectTextBox->setTextAlignment(PO_ALIGN_CENTER_CENTER);
	rectTextBox->doLayout();
	rectTextBox->position.set(428, 180, 0);
	addChild(rectTextBox);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
DictionariesApp::~DictionariesApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void DictionariesApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void DictionariesApp::eventHandler(poEvent *event) {
	
	if( event->message == "rect clicked" ) {
		
		if( event->dictionary.has("rectColor") ) {
			
			poColor rectColor = event->dictionary.getColor("rectColor");
			bigRect->fillColor = rectColor;
		}
		
		if( event->dictionary.has("rectID") ) {
			
			char id[16];
			sprintf(id, "%d", event->dictionary.getInt("rectID"));
			std::string rectID = id;
			std::string text = "You clicked on rectangle " + rectID;
			rectTextBox->setText(text);
			rectTextBox->doLayout();
		}
	}
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void DictionariesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
