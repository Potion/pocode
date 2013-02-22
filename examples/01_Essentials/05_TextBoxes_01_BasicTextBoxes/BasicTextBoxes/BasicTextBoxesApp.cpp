/////////////////////////////////////////
//
// pocode : Basic TextBoxes
//
/////////////////////////////////////////

#include "BasicTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"								// include poTextBox.h to be able to use poTextBox


// APP CONSTRUCTOR. Create all objects here.
BasicTextBoxesApp::BasicTextBoxesApp() {
	
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
	 
	
	// A. poTextBox with plain text ///////////////////////
	
	poTextBox* A = new poTextBox(200, 120);				// Specify the width and height of the text box (optional)
	A->setText("This is a plain text  example...");		// Set the text
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
	if(poGetFont("Helvetica", "Bold")->isValid()) B->setFont( poGetFont("Helvetica", "Bold") );		// Set the font to Helvetica Bold
	B->setTextSize(22);									// Set the text size
	B->textColor = poColor::blue;						// Set the text color
	B->doLayout();										// Always call doLayout() after you make any change
	B->drawBounds = true;								// Show the bounds of the text box
	B->position.set(445, 185, 0);
	addChild( B );
}

// APP DESTRUCTOR. Delete all objects here.
BasicTextBoxesApp::~BasicTextBoxesApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void BasicTextBoxesApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void BasicTextBoxesApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void BasicTextBoxesApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void BasicTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
