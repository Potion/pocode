/////////////////////////////////////////
//
// pocode : Dictionaries
//
/////////////////////////////////////////

#include "DictionariesApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
DictionariesApp::DictionariesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. Create a poDictionary and get data from it ///////////////////////
	
	poDictionary A;												// Create a poDictionary
	A.set("textSize", 25);										// Set an integer value called "textSize"
	A.set("text", "This text and its properties "				// Set a string value called "text"
				  "are stored in the poDictionary A");	
	A.set("textColor", poColor::orange);						// Set a poColor value called "textColor"
	A.set("textPosition", poPoint(60, 180));					// Set a poPoint value called "textPosition"
	
	poTextBox* text = new poTextBox(300, 130);
	text->setText( A.getString("text") );						// Get the value of the string "text" in A
	text->textColor = A.getColor("textColor");					// Get the value of the poColor "textColor" in A
	text->setTextSize( A.getInt("textSize") );					// Get the value of the integer "textSize" in A
	text->doLayout();
	text->position = A.getPoint("textPosition");				// Get the value of the integer "textPosition" in A
	addChild(text);
	
	
	// B. Use of poDictionary in events ///////////////////////
	
	poRandSeed();												// Do this to seed the random color generator
	
	for(int i=1; i < 9; i++) {									// Create 8 rectangles
		
		poRectShape* rect = new poRectShape(30,30);					
		rect->fillColor = poColor::random();					// Assign a random color to each one
		rect->position.set(388 + (i * 40), 280, 0);
		addChild(rect);
		
		poDictionary B;											// Create a poDictionary for each rectangle
		B.set("rectID", i);										// Save the ID in the dictionary
		B.set("rectColor", rect->fillColor);					// Save the random poColor in the dictionary
		
		rect->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, 		// Add a mouse press event to each rectangle with
					   "rect clicked", B);						// a message and a poDictionary attached to it
	}
	
	bigRect = new poRectShape(310, 90);							// Draw a big rectangle that will change color
	bigRect->fillColor = poColor::ltGrey;
	bigRect->position.set(428, 180, 0);
	addChild(bigRect);
	
	rectTextBox = new poTextBox(310, 90);						// Create a text box
	rectTextBox->setText("Click on the colored rectangles to change color");
	rectTextBox->setTextSize(18);
	rectTextBox->setTextAlignment(PO_ALIGN_CENTER_CENTER);
	rectTextBox->doLayout();
	rectTextBox->position.set(428, 180, 0);
	addChild(rectTextBox);
}


// APP DESTRUCTOR. Delete all objects here.
DictionariesApp::~DictionariesApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void DictionariesApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void DictionariesApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void DictionariesApp::eventHandler(poEvent *event) {
	
	if( event->message == "rect clicked" ) {					// Check if the event has that message
		
		if( event->dictionary.has("rectColor") ) {				// Check if the poDictionary of the poEvent
																// contains a key called "rectColor"
			poColor rectColor = event->dictionary.getColor("rectColor");	// Get the value of "rectColor"
			bigRect->fillColor = rectColor;						// Assign the new color to the big rectangle
		}
		
		if( event->dictionary.has("rectID") ) {					// Check if the poDictionary of the poEvent
																// contains a key called "rectID"
			int id = event->dictionary.getInt("rectID");		// Get the value of "rectID"
			char newText[32];									// Create a string
			sprintf(newText, "You clicked on rectangle %d", id); // Append the value of "rectID" to the string
						
			rectTextBox->setText(newText);						// Change the text
			rectTextBox->doLayout();							// Call doLayout() to implement the changes
		}
	}
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void DictionariesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
