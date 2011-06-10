#include "AlignmentApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	return new AlignmentApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

AlignmentApp::AlignmentApp() {
	addModifier(new poCamera2D(poColor::black));
    
    poRectShape* p;
    poOvalShape* o;
    
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            p = new poRectShape(getWindowWidth()/9, getWindowHeight()/9);
            p->position(180+j*(getWindowWidth()/5),150+i*(getWindowHeight()/5));
            p->generateStroke(5).strokeColor(poColor::green);
            switch (i*3+j) {
                case 0: p->setAlignment(PO_ALIGN_TOP_LEFT);  break;
                case 1: p->setAlignment(PO_ALIGN_TOP_CENTER);  break;
                case 2: p->setAlignment(PO_ALIGN_TOP_RIGHT);  break;
                case 3: p->setAlignment(PO_ALIGN_CENTER_LEFT);  break;
                case 4: p->setAlignment(PO_ALIGN_CENTER_CENTER);  break;
                case 5: p->setAlignment(PO_ALIGN_CENTER_RIGHT);  break;
                case 6: p->setAlignment(PO_ALIGN_BOTTOM_LEFT);  break;
                case 7: p->setAlignment(PO_ALIGN_BOTTOM_CENTER);  break;
                case 8: p->setAlignment(PO_ALIGN_BOTTOM_RIGHT);  break;
                default: break;
            }
            addChild(p);
            
            //creating an oval to be a reference for the object alignment
            o = new poOvalShape(10, 10, 20);
            o->position(180+j*(getWindowWidth()/5),150+i*(getWindowHeight()/5)).setAlignment(PO_ALIGN_CENTER_CENTER);
            o->fillColor(poColor::yellow);
            addChild(o);
        }
    }
    addEvent(PO_MOUSE_MOVE_EVENT, this);
}

void AlignmentApp::eventHandler(poEvent *event){
    if(event->type == PO_MOUSE_MOVE_EVENT){
        std::cout<<event->position<<"\n";
        
    }
}

AlignmentApp::~AlignmentApp() {
}