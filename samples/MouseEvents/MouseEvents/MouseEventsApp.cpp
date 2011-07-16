#include "MouseEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new MouseEventsApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 600, 300);
}

void cleanupApplication() {
}

MouseEventsApp::MouseEventsApp() {
	addModifier(new poCamera2D(poColor::grey));
    
    float width = getWindowWidth();
    float height = getWindowHeight();
    rectangle = new poRectShape(width-100,height-100);
    rectangle->position(50,50).calculateBounds();
    rectangle->fillColor(poColor::lt_grey);
    addChild(rectangle);
    
    // note the events that can be added to the main application
    
    // no click necessary
    rectangle->addEvent(PO_MOUSE_ENTER_EVENT, this);
    rectangle->addEvent(PO_MOUSE_LEAVE_EVENT, this);
    addEvent(PO_MOUSE_MOVE_EVENT, this);

    // linked to an object with click events
    rectangle->addEvent(PO_MOUSE_PRESS_EVENT, this);
    rectangle->addEvent(PO_MOUSE_RELEASE_EVENT, this);
    rectangle->addEvent(PO_MOUSE_DRAG_EVENT, this);

    // global click events
    addEvent(PO_MOUSE_DOWN_EVENT, this);
    addEvent(PO_MOUSE_UP_EVENT, this);
    
    // specific click events
    rectangle->addEvent(PO_MOUSE_LEFT, this);
    rectangle->addEvent(PO_MOUSE_MIDDLE, this);
    rectangle->addEvent(PO_MOUSE_RIGHT, this);

}

MouseEventsApp::~MouseEventsApp() {
}

void MouseEventsApp::eventHandler(poEvent *event){
    switch(event->type){
        case PO_MOUSE_ENTER_EVENT:  cout<<"PO_MOUSE_ENTER_EVENT \n";
            break;
        case PO_MOUSE_LEAVE_EVENT:  cout<<"PO_MOUSE_LEAVE_EVENT \n";
            break;
        case PO_MOUSE_MOVE_EVENT:  //cout<<"PO_MOUSE_MOVE_EVENT \n";
            break;
        case PO_MOUSE_PRESS_EVENT:  cout<<"PO_MOUSE_PRESS_EVENT \n";
            break;
        case PO_MOUSE_RELEASE_EVENT:  cout<<"PO_MOUSE_RELEASE_EVENT \n";
            break;
        case PO_MOUSE_DRAG_EVENT:  cout<<"PO_MOUSE_DRAG_EVENT \n";
            break;
        case PO_MOUSE_DOWN_EVENT:  cout<<"PO_MOUSE_DOWN_EVENT \n";
            break;
        case PO_MOUSE_UP_EVENT:  cout<<"PO_MOUSE_UP_EVENT \n";
            break;
        case PO_MOUSE_LEFT:  cout<<"PO_MOUSE_LEFT \n";
            break;
        case PO_MOUSE_MIDDLE:  cout<<"PO_MOUSE_MIDDLE \n";
            break;
        case PO_MOUSE_RIGHT:  cout<<"PO_MOUSE_RIGHT \n";
            break;
        default:
            break;
    }
}