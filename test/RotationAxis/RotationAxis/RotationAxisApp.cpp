#include "RotationAxisApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new RotationAxisApp();
}

void setupApplication() {
	// set resources path
	lookUpAndSetPath("resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "RotationAxis", 100, 100, 600, 300);
}

void cleanupApplication() {
}

RotationAxisApp::RotationAxisApp() {

//	addModifier(new poCamera2D(poColor::black)); // does not work with 2D camera
	
	poPoint camera_position	= poPoint(-300,-150,-450);
	poPoint look_position	= poPoint(-300,-150,0);
	
	poPerspectiveCamera* Cam = new poPerspectiveCamera(45, 1, 1000);
	Cam->cameraPosition( camera_position );
	Cam->lookAtPosition( look_position );
	addModifier(Cam);
	
	animate = true;
	
	angle = 0.f;
	
	rect = new poRectShape("test.png");					//image from Ghostly International - Gold Panda --- Companion
	rect->setAlignment(PO_ALIGN_CENTER_CENTER);
	rect->position = poPoint(getWindowWidth()/2, getWindowHeight()/2);
	rect->rotationAxis = poPoint(1,0,0);
	addChild(rect);
	
	//	press
	//	' ' for stop/start
	//	'x' for x-rotation
	//	'y' for y-rotation 
	//	'z' for z-rotation
	addEvent(PO_KEY_DOWN_EVENT, this);
}

RotationAxisApp::~RotationAxisApp() {
}

void RotationAxisApp::update() {
	
	if(animate)
		rect->rotation = angle+=1.f;	
}

void RotationAxisApp::eventHandler(poEvent *event) {
	
	switch( event->keyChar ){
			
		case ' ':	animate = !animate; break;
		case 'x':	rect->rotationAxis = poPoint(1,0,0); break;
		case 'y':	rect->rotationAxis = poPoint(0,1,0); break;
		case 'z':	rect->rotationAxis = poPoint(0,0,1); break;
		default:	break;
	}
}

void RotationAxisApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
