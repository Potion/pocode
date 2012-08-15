/////////////////////////////////////////
//
// pocode : Control Panel
//
/////////////////////////////////////////

#include "ControlPanelApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"


// APP CONSTRUCTOR. Create all objects here.
ControlPanelApp::ControlPanelApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	control = new poControlPanel( "controlPanel", poColor(0,0,0,0.6), 12 );	// Create a control panel
    // Give it a name, background color (optional) and a text size (optional, default is 11)
	
	// CONTROLS
	// Each control in the control panel has a name and a pointer to a listener poObject
	// So that every time the user touches a button or a slider, a message gets sent
	// to the listener object
	
    control->addToggle( "visible", this );								// Add a toggle button called "visible"
    control->addSliderF( "scale",0.5f, 5.f, this );						// Add a float slider called "scale" with
    // values that go from 0.5 to 5
	
    control->addPointSlider( "loc", poPoint(50,172), poPoint(508,467), this ); // Add a poPoint slider called "loc"
    // and set the min and max poPoint values
    control->addColorSlider( "color", true, this );						// Add a color slider called "color"
    // (it includes R,G,B and alpha sliders)
    control->addKnob( "knob",0.f, 360.f, this );						// Add a knob called "knob"
    // with values that go from 0 to 360
	control->addButton( "rotate!", this );
	
	// To add a list of radio buttons we need to define first a list of button labels
	std::vector<std::string> radioOptions;
	radioOptions.push_back("triangle");
	radioOptions.push_back("quadrilateral");
	radioOptions.push_back("pentagon");
	radioOptions.push_back("circle");
	
	// Now we can create the a list of radio buttons called "shape", by passing the list of strings that we created
	control->addRadio("shape", radioOptions, this);
	
	// Every option in the radio list corresponds to a different number of vertices for an oval shape
	// 3 vertices make a triangle, 4 vertices make a quadrilateral, etc.
	shapeOptions.push_back(3);
	shapeOptions.push_back(4);
	shapeOptions.push_back(5);
	shapeOptions.push_back(40);
	
	// Create an oval shape
	shape = new poOvalShape(40,40,40);
	
	// SAVED VALUES
	// The control panel allows you to save the values in an XML file.
	// Every time the project runs, we are going to set the color, position, scale, etc.
	// to be the values that were previously saved in the control panel
	
	shape->fillColor = control->getColor("color");				// Get a poColor value from the control called "color"
	shape->position = control->getPoint("loc");					// Get a poPoint value from the control called "loc"
    // Get a float value from the control called "scale"
	shape->scale.set(control->getFloat("scale"),control->getFloat("scale"),1);
	shape->visible = control->getBool("visible");				// Get a boolean from the control called "visible"
	shape->rotation = control->getFloat("knob");                // Get a float value from the control called "knob"
	
	int optionID = control->getInt("shape");			// Get an integer value from the radio buttons called "shape"
    // The integer is the item ID in the vector list of options
	shape->reshape(40,40, shapeOptions[optionID]);		// Reshape the oval shape based on the option
	
    control->addInputTextBox( "print", this );                  // Add an input text box called "print"
	control->addInputTextBox( "text", this );					// Add an input text box called "text"
	control->addSliderI( "textSize", 10, 30, this );			// Add an integer slider called "size"
    // with values from 10 to 30
	
	text = new poTextBox(450,295);
	text->setText(control->getString("text"));					// Get a string value from the control called "text"
	text->setFont(poGetFont("Helvetica", "Bold"));
	text->setTextSize(control->getInt("textSize"));				// Get a integer value from the control called "size"
	text->textColor = poColor::black;
	text->setTextAlignment(PO_ALIGN_CENTER_CENTER);
	text->doLayout();
	text->position.set(50, 172, 0);
	
	// Add all the children in order, so that the control panel is on top of everything;
	addChild(shape);
	addChild(text);
	addChild(control);
    
    addEvent(PO_MOUSE_MOVE_EVENT, this);
}

// APP DESTRUCTOR. Delete all objects here.
ControlPanelApp::~ControlPanelApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void ControlPanelApp::update() {
    
}

// DRAW. Called once per frame. Draw objects here.
void ControlPanelApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void ControlPanelApp::eventHandler(poEvent *event) {
	
    if( event->type == PO_MOUSE_MOVE_EVENT ) {
        char outputMessage[64];
        if(event->globalPosition.x < 0 || event->globalPosition.x > getWindowWidth() ||
           event->globalPosition.y < 0 || event->globalPosition.y > getWindowHeight()) {
            sprintf(outputMessage, "The mouse is outside of the window!");
        }
        else {
            sprintf(outputMessage, "The mouse is at %.0f, %.0f", event->globalPosition.x, event->globalPosition.y);
        }
        control->setString("print", outputMessage);        // set the string for the poInputTextBox called "print"
    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void ControlPanelApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	// CONTROL PANEL MESSAGES
	// Every time the user modifies one of the controls in the control panel,
	// The listener object receives a message with the name of the control UI that was modified
	
	if ( msg == "loc" ) {
        shape->position = control->getPoint("loc");
    }
	
	if ( msg == "scale" ) {
        shape->scale.set(control->getFloat("scale"), control->getFloat("scale"), 1);
    }
	
	if ( msg == "color" ) {
        shape->fillColor = control->getColor("color");
    }
	
	if ( msg == "visible" ) {
        shape->visible = control->getBool("visible");
    }
	
	if ( msg == "knob" ) {
		shape->rotation = control->getFloat("knob");
	}
	
	if ( msg == "shape" ) {
		int optionID = control->getInt("shape");
		shape->reshape(40,40, shapeOptions[optionID]);
	}
	
	if ( msg == "rotate!" ) {
		float currentRotation = shape->rotation;
		shape->rotation -= 360;
		shape->rotationTween
		.set(currentRotation)
		.setTweenFunction(PO_TWEEN_LINEAR_FUNC)
		.setDuration(1)
		.start();
	}
	
	if ( msg == "text" ) {
		text->setText(control->getString("text"));
		text->doLayout();
	}
	
	if ( msg == "textSize" ) {
        text->setTextSize(control->getInt("textSize"));
		text->doLayout();
    }
}
