#include "colorPickerApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	// make the root poObject for our window
	return new colorPickerApp();
}

void setupApplication() {
	// go up from the present working directory (pwd),
	// find a directory called resources
	// then change into it for the pwd
	lookUpAndSetPath("resources");
	
	// create the application window
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "colorPicker", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

colorPickerApp::colorPickerApp() {
	addModifier(new poCamera2D(poColor::black));

	// load an image from the pwd
	image = new poImage("bird.png");
	// make the window fit the image
	applicationReshapeWindow(applicationCurrentWindow(), poRect(100,100,image->width(),image->height()));
	
	// make a shape to hold our image
	// with this constructor it will match the image size
	poRectShape *shape = new poRectShape(image);
	// add a move event to the shape, but handle it in this object
	shape->addEvent(PO_MOUSE_MOVE_EVENT, this);
	// put it into the drawing tree
	addChild(shape);
	
	// this will follow the mouse around, showing the current color
	indicator = new poOvalShape(100,100,100);
	// configure the oval to be hollow and centered around its origin
	indicator->enableFill(false).strokeColor(poColor::white).generateStroke(12).position(200,200).alignment(PO_ALIGN_CENTER_CENTER);
	// and put it into the drawing tree
	addChild(indicator);
	
	// make a text box to show the RGBA values under the mouse
	readout = new poTextBox(250,25);
	// use system font Helvetica.
	// the font could also be a .otf, .ttf or .dfont file
	readout->font(poFont("Helvetica",15,PO_FONT_BOLD))
		.textColor(poColor::red)
		.textAlignment(PO_ALIGN_CENTER_CENTER)
		// convenience to add a rectangle behind the text
		.buttonize(poColor::black, poColor::white, 2)
		// align the text box so its local origin is in its bottom center
		.alignment(PO_ALIGN_BOTTOM_CENTER)
		// center it on the bottom of the screen
		.position(getWindowWidth()/2.f,getWindowHeight());
	// and add it to the tree
	addChild(readout);
}

colorPickerApp::~colorPickerApp() {
	// clean up the image
	delete image;
	
	// all the current children of this object will get deleted automatically
}

void colorPickerApp::eventHandler(poEvent *event) {
	// we're only interested in mouse move events
	// you will only get events that you register for though
	// so this check if pro-forma only
	if(event->type == PO_MOUSE_MOVE_EVENT) {
		// find the pixel value for the spot under the mouse
		// local_position is mouse coords within the coordinate system of the event source
		poColor pixel = image->getPixel(event->local_position);
		
		char str[255];
		sprintf(str, "% 3d % 3d % 3d % 3d", int(pixel.R*255), int(pixel.G*255), int(pixel.B*255), int(pixel.A*255));
		// you must call layout manually after changing the text of a text box
		readout->text(str).layout();
		
		// move the indicator with the mouse
		indicator->position(event->position);
		// and color it to match the pixel
		indicator->strokeColor(poColor(pixel,0.8));
	}
}
