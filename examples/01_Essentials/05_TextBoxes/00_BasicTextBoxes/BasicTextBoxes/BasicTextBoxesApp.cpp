/////////////////////////////////////////
//
// pocode : Basic TextBoxes
//
/////////////////////////////////////////

#include "BasicTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"										// include poTextBox.h to be able to use poTextBox


// APP CONSTRUCTOR.
// Create all objects here.
BasicTextBoxesApp::BasicTextBoxesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	 
	
	// A. poTextBox with plain text ///////////////////////
	
	poTextBox* A = new poTextBox(200, 120);				// Specify the width and height of the text box (optional)
	A->setText("This is a plain text  example...");		// Set the text
	A->setFont( poGetFont("Helvetica", "Regular") );	// Set the font, always
														// You can create a new poFont by calling poGetFont("FontFamilyName", "FontStyle")
														// Make sure the font is installed in your machine
	A->setTextSize(18);									// Set the text size
	A->textColor = poColor::black;						// Set the text color
	A->doLayout();										// Always call doLayout() after you make any change
	A->drawBounds = true;								// Show the bounds of the text box
	A->position.set(75, 185, 0);
	addChild( A );
	
	
	// B. poTextBox color, style and text alignment ///////////////////////
	
	poTextBox* B = new poTextBox(280, 120);
	B->setText("This is an example of bold text aligned to the bottom right...");	// Set the text
	B->setTextAlignment(PO_ALIGN_BOTTOM_RIGHT);			// Set the text alignment within the text box
	B->setFont( poGetFont("Helvetica", "Bold") );		// Set the font to Helvetica Bold
	B->setTextSize(22);									// Set the text size
	B->textColor = poColor::blue;						// Set the text color
	B->doLayout();										// Always call doLayout() after you make any change
	B->drawBounds = true;								// Show the bounds of the text box
	B->position.set(445, 185, 0);
	addChild( B );
}

// APP DESTRUCTOR
// Delete all objects here. (optional)
BasicTextBoxesApp::~BasicTextBoxesApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void BasicTextBoxesApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void BasicTextBoxesApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void BasicTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
