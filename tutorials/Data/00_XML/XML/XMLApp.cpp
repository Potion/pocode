/////////////////////////////////////////
//
// LESSON : XML
//
/////////////////////////////////////////

#include "XMLApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
using namespace boost;

// APP CONSTRUCTOR.
// Create all objects here.
XMLApp::XMLApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// Loading an XML file ///////////////////////
	
	poXMLDocument doc = poXMLDocument("data.xml");			// Create a poXMLDocument
															// by reading the file "data.xml"
	
	poXMLNode rootNode = doc.getRootNode();					// Get the very first node at the root level
															// of the XML structure (in this case <xml>)
	
	// A. Reading an XML Node ///////////////////////
	
	poXMLNode nodeA = rootNode.getFirstChild();				// Get the first child node of the root node
	
	std::string fontFamilyA = nodeA.getStringAttribute("fontFamily"); // Get the attribute called "fontFamily" as a string
	
	int textSizeA  = nodeA.getIntAttribute("textSize");		// Get the attribute called "textSize" as an integer
	
	float colorR = nodeA.getFloatAttribute("colorR");		// Get the attribute called "colorR" as a float
	float colorG = nodeA.getFloatAttribute("colorG");		// Get the attribute called "colorG" as a float
	float colorB = nodeA.getFloatAttribute("colorB");		// Get the attribute called "colorB" as a float
	
	std::string textA = nodeA.getInnerString();				// Get the content of node A as a string
	
	printf("%s\n", fontFamilyA.c_str());
	
	poFont* fontA = poGetFont(fontFamilyA, "Regular");		// Set the font based on the "fontFamily" attribute
	
	if ( fontA->isValid() ) {								// Check that the font is valid
															// otherwise the poTextBox will complain
		
		poTextBox* textBoxA = new poTextBox(195, 140);		// Create a TextBox to display the string
		textBoxA->setText( textA );							// Set the text based on the content of node <A>
		textBoxA->setFont( fontA );							
		textBoxA->setTextSize( textSizeA );					// Set the size based on the "textSize" attribute
		textBoxA->textColor.set(colorR, colorG, colorB);	// Set the text color based on the color attriutes
		textBoxA->doLayout();
		textBoxA->position.set(55, 175, 0);
		addChild(textBoxA);
	}
	
	// B. Children of an XML Node ///////////////////////
	
	poXMLNode nodeB = rootNode.getChild("B");				// Get the first child node named "B"
	
	int numChildrenOfB = nodeB.getNumChildren();			// Get the number of children of node <B>
	
	for( int i=0; i < numChildrenOfB; i++ ) {				// Loop through the children of <B>
		
		poXMLNode node = nodeB.getChild(i);					// Get a child of node <B> by index (0,1,2,etc.)
		
		float size = node.getInnerFloat();					// Get the content of the child node as a float
		
		poRectShape* rect = new poRectShape(size, 20);		// Create a rectangle with size based on the
															// value of the XML node
		rect->fillColor.set(0.8, 0.6, 0);
		rect->position.set(310, 180 + (25*i), 0);
		addChild(rect);
		
		std::string stringValue = node.getInnerString();	// Get the content of the child node as a string
		
		poTextBox* text = new poTextBox(50, 20);			// Create a poTextBox to display the string
		text->setText( stringValue );						// Set the text based on the value of the node
		text->setFont( poGetFont("Helvetica", "Regular") );
		text->setTextSize(13);
		text->textColor = poColor::white;
		text->doLayout();
		text->position.set(312, 180 + (25*i), 0);
		addChild(text);
	}
	
	
	// C. Siblings of an XML Node ///////////////////////
	
	poXMLNode nodeC = nodeB.getNextSibling();				// Get the next sibling of node <B> (that is <C>)
	
	poXMLNode childNodeC = nodeC.getFirstChild();			// Get the first child node of <C>
	
	float x = 590;											// Set an initial x position of the circles
															// that we are going to draw, so we can
															// increase it for each new circle
	
	while (childNodeC.isValid()) {							// At every loop, check if childOfNodeC
															// is valid, which means it exists in the file.
		
		float size = childNodeC.getFloatAttribute("size"); // Get the value of the node as a float
		
		int strokeWidth = 0;
		if(childNodeC.hasAttribute("stroke")) {				// Check if the node has an attribute "stroke"
			
			strokeWidth = childNodeC.getIntAttribute("stroke"); // If so, save its value into an integer
		}
		
		poOvalShape* oval = new poOvalShape(size, size, size); // Set the size of the circle
		oval->fillColor.set(0.8, 0.6, 0);
		oval->generateStroke(strokeWidth);					// Set the stroke width (0 if "stroke" wasn't found)
		oval->strokeColor = poColor::black;
		oval->position.set(x, 240, 0);
		addChild(oval);
		
		childNodeC = childNodeC.getNextSibling();			// Make childNodeC equal to its sibling node
															// If it doesn't exists, the loop will stop
		x += 30;
	}
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
XMLApp::~XMLApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void XMLApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void XMLApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void XMLApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
