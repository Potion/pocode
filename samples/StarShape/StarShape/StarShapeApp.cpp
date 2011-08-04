#include "StarShapeApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new StarShapeApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "StarShape", 100, 100, 600, 300);
}

void cleanupApplication() {
}

StarShapeApp::StarShapeApp() {
	addModifier(new poCamera2D(poColor::black));
    
    addEvent(PO_KEY_DOWN_EVENT, this);
    
    numPoints = 5;
    radius = 100;
    depth = 40; 
    
    s = new poStarShape(radius, numPoints, depth); 
    s->position = poPoint(300,150);
    addChild(s);
}

StarShapeApp::~StarShapeApp() {
}

void StarShapeApp::draw(){
    s->reshape(radius, numPoints, depth);
}

void StarShapeApp::eventHandler(poEvent *event) {
	if( event->type == PO_KEY_DOWN_EVENT ){
        switch (event->keyChar) {
                
            case '>':	numPoints++;	break;
                
            case '<':	numPoints--;	break;
                
            case 'f':	radius++;	break;
                
            case 'v':	radius--;	break;
                
            case 'g':	depth++;	break;
                
            case 'b':	depth--;	break;
                
            default:	break;
        }
    }
}

void StarShapeApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
