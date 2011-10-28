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
    rectangle->name = "rectangle";
    rectangle->fillColor = poColor::lt_grey;
	rectangle->position = poPoint(50,50, 0.f);
    addChild(rectangle);
	 
	rectangle->addEvent(PO_MOUSE_DOWN_EVENT, this);
    rectangle->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);

    rectangle->addEvent(PO_MOUSE_UP_EVENT, this);
    rectangle->addEvent(PO_MOUSE_DRAG_EVENT, this);

    addEvent(PO_KEY_DOWN_EVENT, this);
    addEvent(PO_KEY_UP_EVENT, this); 

}

MouseEventsApp::~MouseEventsApp() {}

void MouseEventsApp::eventHandler(poEvent *event){

    switch(event->type){
		case PO_MOUSE_DOWN_EVENT:
			cout<<"PO_MOUSE_DOWN_EVENT \n";
			break;
        case PO_MOUSE_ENTER_EVENT:   
			cout<<"PO_MOUSE_ENTER_EVENT \n";
            break;
        case PO_MOUSE_LEAVE_EVENT:  
			cout<<"PO_MOUSE_LEAVE_EVENT \n";
            break;
		case PO_MOUSE_OVER_EVENT:	
			cout<<"PO_MOUSE_OVER_EVENT \n";
			break; 
        case PO_MOUSE_DOWN_INSIDE_EVENT:
			cout<<"PO_MOUSE_DOWN_INSIDE_EVENT " + event->source->name + "\n"; 
            break;
        case PO_MOUSE_UP_EVENT:  
			cout<<"PO_MOUSE_UP_EVENT \n";
            break;
        case PO_MOUSE_DRAG_EVENT:  
			cout<<"PO_MOUSE_DRAG_EVENT \n";  
            break;
        case PO_KEY_DOWN_EVENT:
            if ( time(0)%2 == 0 )
                moveChildToFront(rectangle);
            else
                moveChildToBack(rectangle);
            cout<<"PO_KEY_DOWN_EVENT \n";
			break;
        case PO_KEY_UP_EVENT:
			cout<<"PO_KEY_UP_EVENT \n";
			break;
        default:
            break;
    }
}