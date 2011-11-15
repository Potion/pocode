/////////////////////////////////////////
//
// LESSON : Basic TextBoxes
//
/////////////////////////////////////////

#include "BasicTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poShapeBasics2D.h"
#include "poTextBox.h"									// include poTextBox.h to be able to use poTextBox

poObject *createObjectForID(uint uid) {
	return new BasicTextBoxesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicTextBoxes", 100, 100, 800, 600);
}

void cleanupApplication() {
}

BasicTextBoxesApp::BasicTextBoxesApp() {
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. poTextBox with plain text ///////////////////////
	
	poTextBox* A = new poTextBox(200, 120);					// Specify the width and height of the text box (optional)
	A->setText("This is a plain text  example...");			// Set the text
	A->setFont( poGetFont("Helvetica") );					// Set the font, always
															// You can create a new poFont by calling poFont("FontFamilyName")
															// Make sure the font is installed in your machine
	A->setTextSize(18);										// Set the text size
	A->textColor = poColor::black;							// Set the text color
	A->doLayout();											// Always call doLayout() after you make any change
	A->drawBounds = true;									// Show the bounds of the text box
	A->position.set(75, 248, 0);
	addChild( A );
	
	
	// B. poTextBox with rich text ///////////////////////
	
	poTextBox* B = new poTextBox(200, 120);
	B->setText("This is a <b>rich text</b> example...");
	B->setRichText( true );
	B->setFont( poGetFont("Helvetica", 18) );
	B->setFont( poGetFont("Helvetica-Bold", 18), PO_TEXT_BOLD );
	B->textColor = poColor::black;
	B->doLayout();
	B->drawBounds = true;
	B->position.set(445, 248, 0);
	addChild( B );
}

BasicTextBoxesApp::~BasicTextBoxesApp() {
}

void BasicTextBoxesApp::eventHandler(poEvent *event) {
	
}

void BasicTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
