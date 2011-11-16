/////////////////////////////////////////
//
// LESSON : Advanced TextBoxes
//
/////////////////////////////////////////

#include "AdvancedTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poShapeBasics2D.h"
#include "poTextBox.h"									// include poTextBox.h to be able to use poShapes

poObject *createObjectForID(uint uid) {
	return new AdvancedTextBoxesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedTextBoxes", 100, 100, 800, 600);
}

void cleanupApplication() {
}

AdvancedTextBoxesApp::AdvancedTextBoxesApp() {
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. poTextBox with alignment? ///////////////////////
	
	poTextBox* A = new poTextBox(200, 120);
	A->setText("This is a plain text  example...");
	A->setFont( poGetFont("Helvetica") );
	A->setTextSize(18);
	A->textColor = poColor::red;
	A->doLayout();
	A->drawBounds = true;
	A->position.set(75, 248, 0);
//	A->setAlignment(PO_ALIGN_CENTER);			// Alignment of the text can be left, center, right or justified
	addChild( A );
	
	
	// B. poTextBox with  ///////////////////////
	
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

AdvancedTextBoxesApp::~AdvancedTextBoxesApp() {
}

void AdvancedTextBoxesApp::eventHandler(poEvent *event) {
	
}

void AdvancedTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
