#include "TweenApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new TweenApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Tween", 100, 100, 600, 300);
}

void cleanupApplication() {
}

TweenApp::TweenApp() {
	addModifier(new poCamera2D(poColor::black));
	
	zoomed = false;
    
    pho = new poRectShape(50,50);
    pho->generateStroke(2).strokeColor(poColor::cyan).fillColor(poColor::dk_grey);
    pho->position(20,20,0);
    addChild(pho);
	
	banh = new poOvalShape(50,50,3);
	banh->generateStroke(2).strokeColor(poColor::yellow).fillColor(poColor::dk_grey);
	banh->position(150,150,0);
	banh->addEvent(PO_MOUSE_PRESS_EVENT, this);
    addChild(banh);

	mi = new poOvalShape(50,50,100);
	mi->generateStroke(2).strokeColor(poColor::magenta).fillColor(poColor::dk_grey);
	mi->position(450,150,0);
	mi->addEvent(PO_MOUSE_PRESS_EVENT, this);
    addChild(mi);
	
    // LERP (linear interpolation easing is default)
    pho->position_tween							//	position tween
        .set(poPoint(530,20))					//	destination value for tween
        .setRepeat(PO_TWEEN_REPEAT_PINGPONG)	//	NONE, REGULAR, PINGPONG
        .setExtraValues(.90, .05)				//	(e1, e2) step e1 each time, stop @ e2
        .start();								//	begin the tween
	
	// Linear tween
	banh->rotation_tween						//	rotation tween
		.set(360)								//	rotation measured in degrees
		.setRepeat(PO_TWEEN_REPEAT_REGULAR)		//	repeat is set to loop
		.setTweenFunction(linearFunc)			//	linear (no ease)
		.setDuration(2)							//	Easing functions require a duration
		.start();
}

TweenApp::~TweenApp() {
}

void TweenApp::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_PRESS_EVENT){
		
		poPoint val;
		
		if(!zoomed)								//	sets an end value for the scale tween so we can zoom in and then back out
			val = poPoint(2,2,1);				//	all three arguments needed for a scale tween (since z coord defaults to 0)
		else
			val = poPoint(1,1,1);
		zoomed = !zoomed;
		
		// non repeating
		event->source->scale_tween				//	scale tween the object clicked on
		.set(val)								//	scaling is done with a vector
		.setRepeat(PO_TWEEN_REPEAT_NONE)		//	no repeat
		.setTweenFunction(linearFunc)			//	linear (no ease)
		.setDuration(2)							//	Easing functions require a duration
		.start();
	}
}

void TweenApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
