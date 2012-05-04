/////////////////////////////////////////
//
// pocode : Advanced TextBoxes
//
/////////////////////////////////////////

#include "AdvancedTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes
#include "poTextBox.h"								// include poTextBox.h to be able to use poTextBox


// APP CONSTRUCTOR. Create all objects here.
AdvancedTextBoxesApp::AdvancedTextBoxesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	// A. poTextBox with rich text ///////////////////////
	
	poTextBox* A = new poTextBox(280, 120);
	A->setRichText(true);
	A->setFont(poGetFont("OpenSans-Italic.ttf"), PO_TEXT_ITALIC);
	A->setFont(poGetFont("OpenSans-Bold.ttf"), PO_TEXT_BOLD);
	A->setFont(poGetFont("ATLASSOL.TTF"), "retro");
	A->setFont(poGetFont("bitwise.ttf"), "computer");
	A->setFont(poGetFont("SaloonExt.ttf"), "western");
	A->setFont(poGetFont("Old Script.ttf"), "calligraphy");
	A->setText("This is a poTextBox with rich text. You can use html-style tags to make text <i>italic</i>, <b>bold</b>, <retro>retro</retro>, <computer>computery</computer>, <western>western</western>, <calligraphy>Calligraphic</calligraphy>.");
	A->setTextSize(18);
	A->textColor = poColor::black;
	A->doLayout();
	A->position.set(75, 180, 0);
	addChild(A);
	
	
	// B. poTextBox leading and tracking ///////////////////////
	
	poTextBox* B = new poTextBox(280, 120);
	B->setText(poToUpperCase("This is a regular text with some leading and tracking."));
	B->setTextSize(16);
	B->textColor = poColor::black;
	B->setLeading(2.0);
	B->setTracking(1.45);
	B->setTextAlignment(PO_ALIGN_CENTER_CENTER);
	B->doLayout();
	B->position.set(445, 180, 0);
	addChild(B);
}

// APP DESTRUCTOR. Delete all objects here.
AdvancedTextBoxesApp::~AdvancedTextBoxesApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void AdvancedTextBoxesApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void AdvancedTextBoxesApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void AdvancedTextBoxesApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void AdvancedTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
