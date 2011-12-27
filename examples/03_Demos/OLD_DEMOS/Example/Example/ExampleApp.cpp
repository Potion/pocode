#include "ExampleApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poGeometryMask.h"
#include "poShapeBasics2D.h"

#include <boost/format.hpp>

boost::format text_fmt("mouse coords: % 4.0f % 4.0f");

poObject *createObjectForID(uint uid) {
	// make the application for this run
	return new ExampleApp();
}

void setupApplication() {
	// set the path to a 'resources' folder next to or above the app
	lookUpAndSetPath("resources");
	// other options are WINDOW_TYPE_BORDERLESS and WINDOW_TYPE_NORMAL_FULLSCREEN
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Example", 100, 100, 800, 800);
}

void cleanupApplication() {
}

ExampleApp::ExampleApp() {
	// you must have at least 1 camera
	addModifier(new poCamera2D());
	
	// make a rectangle with the size of the requested image
	poRectShape *shape = addChild(new poRectShape("img1.png"));
	shape->drawBounds = true;
	// re-orient the shape around its own center point
	shape->alignment(PO_ALIGN_CENTER_CENTER);
	// and place it in the center of the screen
	shape->position = getWindowCenter();
	
	// attach enter and leave events to the shape
	shape->addEvent(PO_MOUSE_ENTER_EVENT, this);
	shape->addEvent(PO_MOUSE_LEAVE_EVENT, this);
	
	// up and down events
	shape->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	shape->addEvent(PO_MOUSE_UP_EVERYWHERE_EVENT, this);

	// a drag event starts when you click on an object
	shape->addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	// key events will go to all interested parties
	addEvent(PO_KEY_DOWN_EVENT, this);
	// and if you care about the window being resized
	addEvent(PO_WINDOW_RESIZED_EVENT, this);
	
	// initialize this variable for safty
	mask = NULL;
	
	// initial text for the mouse coords
	std::string coords = (text_fmt % 0 % 0).str();
	
	// create a text box with a blah size
	poTextBox *text = addChild(new poTextBox(400,20));
	text->text(coords);
	// you must call layout after reconfiguring a text box
	text->layout();
	
	// its a ... mouse move event
	addEvent(PO_MOUSE_MOVE_EVENT, this);
	
	// also hopefully self-explanitory
	shape->addEvent(PO_MOUSE_DOWN_OUTSIDE_EVENT, this);
	
	// all poObjects have a couple of property tweens
	// see poObject.h and also poTween.h
	shape->rotation_tween.setTweenFunction(linearFunc).setDuration(10.f).setRepeat(PO_TWEEN_REPEAT_REGULAR);

	// a global dictionary that reads from and writes to "common.xml"
	poDictionary *common = poCommon::get();
	if(common->has("rotation"))
		shape->rotation = common->getFloat("rotation");
}

ExampleApp::~ExampleApp() {
	// record the final rotation of the shape
	poCommon::get()->setFloat("rotation", getChild(0)->rotation);
}

void ExampleApp::eventHandler(poEvent *event) {
	// handle key events
	if(event->type == PO_KEY_DOWN_EVENT) {
		// return key
		if(event->keyCode == PO_RETURN_KEY) {
			// toggle fullscreen the window
			poWindow *window = applicationCurrentWindow();
			applicationMakeWindowFullscreen(window, !window->isFullscreen());
		}
	}
	
	// re-center the image
	else if(event->type == PO_WINDOW_RESIZED_EVENT) {
		getChild(0)->position = getWindowCenter();
	}
	
	// turn on image border
	else if(event->type == PO_MOUSE_ENTER_EVENT) {
		poShape2D *shape = static_cast<poShape2D*>(event->source);
		shape->strokeColor = poColor::orange;
		shape->generateStroke(20);
	}
	
	// turn off image border
	else if(event->type == PO_MOUSE_LEAVE_EVENT) {
		poShape2D *shape = static_cast<poShape2D*>(event->source);
		shape->generateStroke(0);
	}
	
	// start masking the image
	else if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {
		mask = new poOvalShape(100,100,50);
		mask->position = event->local_position;
		event->source->addModifier(new poGeometryMask(mask));
	}
	
	// stop masking the image
	else if(event->type == PO_MOUSE_UP_EVERYWHERE_EVENT) {
		event->source->removeModifier(0, true);
		mask = NULL;
	}
	
	// we're dragging the mask around!
	else if(event->type == PO_MOUSE_DRAG_EVENT) {
		mask->position = event->local_position;
	}
	
	// display the mouse coordinates
	else if(event->type == PO_MOUSE_MOVE_EVENT) {
		getChildAs<poTextBox>(1)->text((text_fmt % event->position.x % event->position.y).str())->layout();
	}

	// toggle image rotation
	else if(event->type == PO_MOUSE_DOWN_OUTSIDE_EVENT) {
		bool running = event->source->rotation_tween.isRunning();
		if(running)	event->source->rotation_tween.stop();
		else		event->source->rotation_tween.set(event->source->rotation + 360).start();
			
	}
	
}

void ExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
