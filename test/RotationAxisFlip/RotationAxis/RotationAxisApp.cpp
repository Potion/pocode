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

//	addModifier(new poCamera2D( poColor::black ));	// does not work with 2D camera
	
//	addModifier(new poOrthoCamera(getWindowWidth(), getWindowHeight(), -500, 500));	// works as ortho 2D

	poPoint camera_position	= poPoint(-getWindowWidth()/2,-getWindowHeight()/2,-getWindowWidth()*3/4.f);
	poPoint look_position	= poPoint(-getWindowWidth()/2,-getWindowHeight()/2,0);
	
	poPerspectiveCamera* Cam = new poPerspectiveCamera(45, .001, 1000);
	Cam->cameraPosition( camera_position );
	Cam->lookAtPosition( look_position );
	addModifier(Cam);
	
	rect = new poRectShape("test.png");					//image from Ghostly International - Gold Panda --- Companion
	rect->setAlignment(PO_ALIGN_CENTER_CENTER);
	rect->position = poPoint(getWindowWidth()/2, getWindowHeight()/2);
	rect->rotationAxis = poPoint(1,0,0);
	addChild(rect);
	
	//	press
	//	' ' for flip on current axis
	//	'x' for x-rotation
	//	'y' for y-rotation 
	//	'z' for z-rotation
	addEvent(PO_KEY_DOWN_EVENT, this);
}

RotationAxisApp::~RotationAxisApp() {
}

void RotationAxisApp::update() {
}

void RotationAxisApp::flip() {
	
	float flip_duration = 1.f;

	// flip rotation
	rect->rotationTween
		.set(rect->rotation+180)
		.setDuration(flip_duration)
		.setTweenFunction(PO_TWEEN_QUAD_INOUT_FUNC)
		.start();
	
	// move back for rotation
	rect->positionTween
		.set(poPoint(getWindowWidth()/2, getWindowHeight()/2, rect->getHeight()*5/8.f))
		.setDuration(flip_duration/2.f)
		.setTweenFunction(PO_TWEEN_SINUS_INOUT_FUNC)
		.setRepeat(PO_TWEEN_REPEAT_PINGPONG,1)
		.start();	
	
	// dim color when rotating
	rect->fillColorTween
		.set(poColor(.5f,.5f,.5f,1.f))
		.setDuration(flip_duration/2.f)
		.setTweenFunction(PO_TWEEN_SINUS_INOUT_FUNC)
		.setRepeat(PO_TWEEN_REPEAT_PINGPONG,1)
		.start();	
}

void RotationAxisApp::eventHandler(poEvent *event) {
	
	switch( event->keyChar ){
		
		case ' ':	flip(); break;
		case 'x':	rect->rotationAxis = poPoint(1,0,0); break;
		case 'y':	rect->rotationAxis = poPoint(0,1,0); break;
		case 'z':	rect->rotationAxis = poPoint(0,0,1); break;
		default:	break;
	}
}

void RotationAxisApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
