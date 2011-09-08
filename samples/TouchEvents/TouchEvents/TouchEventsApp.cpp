#include "TouchEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TouchEventsApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_FULLSCREEN, "TouchEvents", 1400, 500, 1920, 975);
}

void cleanupApplication() {
}

TouchEventsApp::TouchEventsApp() {
	addModifier(new poCamera2D(poColor::dk_grey));
	
    kinect = new KinectTouch( 1920, 975,
							 63, 149,
							 590, 151,
							 575, 413,
							 72, 416,
                             5, true,		// scale factor, and flip true
							 19, 30, 12,
							 5);	// values of touch fidelity
	
	float width = getWindowWidth();
	float height = getWindowHeight();
	
	rectangle = new poRectShape(width/2.f,height/2.f);
	rectangle->fillColor = poColor::grey;
	rectangle->alignment(PO_ALIGN_CENTER_CENTER);
	rectangle->position = poPoint(width/2.f, height/2.f);
	addChild(rectangle);
	
	rectangle->addEvent(PO_TOUCH_BEGAN_EVENT, this);
	rectangle->addEvent(PO_TOUCH_ENDED_EVENT, this);
	rectangle->addEvent(PO_TOUCH_INSIDE_EVENT, this);
	rectangle->addEvent(PO_TOUCH_MOVED_EVENT, this);
	rectangle->addEvent(PO_TOUCH_OVER_EVENT, this);
	
	eventTB = new poTextBox(500, 100);
	eventTB->text("waiting for event");
	eventTB->alignment(PO_ALIGN_CENTER_CENTER);
	eventTB->textAlignment(PO_ALIGN_CENTER_CENTER);
	eventTB->position = poPoint(width/2.f, height/2.f);
	eventTB->layout();
	addChild(eventTB);
	
	marker = new poOvalShape(20.f,20.f,50);
	marker->fillColor = poColor::magenta;
	addChild(marker);
	
}

TouchEventsApp::~TouchEventsApp() {
}

void TouchEventsApp::eventHandler(poEvent *event) {
	switch(event->type){
        case PO_TOUCH_BEGAN_EVENT:  
			cout<<"PO_TOUCH_BEGAN_EVENT @ "<< event->position <<"\n";
			eventTB->text("PO_TOUCH_BEGAN_EVENT");
			eventTB->layout();
            break;
        case PO_TOUCH_ENDED_EVENT:  
			cout<<"PO_TOUCH_ENDED_EVENT @ "<< event->position <<"\n";
			eventTB->text("PO_TOUCH_ENDED_EVENT");
			eventTB->layout();
            break;
		case PO_TOUCH_INSIDE_EVENT:	
			cout<<"PO_TOUCH_INSIDE_EVENT @ "<< event->position <<"\n";
			eventTB->text("PO_TOUCH_INSIDE_EVENT");
			eventTB->layout();
			break;
		case PO_TOUCH_MOVED_EVENT:	
			cout<<"PO_TOUCH_MOVED_EVENT @ "<< event->position <<"\n";
			eventTB->text("PO_TOUCH_MOVED_EVENT");
			eventTB->layout();
			break;
		case PO_TOUCH_OVER_EVENT:	
			cout<<"PO_TOUCH_OVER_EVENT @ "<< event->position <<"\n";
			eventTB->text("PO_TOUCH_OVER_EVENT");
			eventTB->layout();
			break;
        default:
            break;
    }
	marker->position = event->position;

}

void TouchEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}