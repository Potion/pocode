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

TweenApp::TweenApp()
:	simple_tween(&simple_tween_val)
,	simple_tween_val(0.f)
{
	addModifier(new poCamera2D(poColor::black));
	
	zoomed = false;
    
    pho = new poRectShape(50,50);
    pho->position = poPoint(20,20,0);
	pho->fillColor = poColor::dk_grey;
	pho->strokeColor = poColor::cyan;
    pho->generateStroke(2);
    addChild(pho);
	
	banh = new poOvalShape(50,50,3);
	banh->position = poPoint(150,150,0);
	banh->fillColor = poColor::dk_grey;
	banh->strokeColor = poColor::yellow;
	banh->generateStroke(2);
	banh->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    addChild(banh);

	mi = new poOvalShape(50,50,100);
	mi->position = poPoint(450,150,0);
	mi->fillColor = poColor::dk_grey;
	mi->strokeColor = poColor::magenta;
	mi->generateStroke(2);
	mi->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    addChild(mi);
	
    // SLERP (mid-point easing is default)
    pho->positionTween							//	position tween
        .set(poPoint(530,20))					//	destination value for tween
        .setRepeat(PO_TWEEN_REPEAT_PINGPONG)	//	NONE, REGULAR, PINGPONG
        .setExtraValues(.90, .05)				//	(e1, e2) step e1 each time, stop @ e2
        .start();								//	begin the tween
	
	// Linear tween
	banh->rotationTween						//	rotation tween
		.set(360)								//	rotation measured in degrees
		.setRepeat(PO_TWEEN_REPEAT_REGULAR)		//	repeat is set to loop
		.setTweenFunction(linearFunc)			//	linear (no ease)
		.setDuration(2)							//	Easing functions require a duration
		.setDelay(2,true)
		.start();

	simple_tween.set(1.f).setTweenFunction(linearFunc).setDuration(1.f).setRepeat(PO_TWEEN_REPEAT_PINGPONG).setNotification(this,"done").start();
}

TweenApp::~TweenApp() {
}

void TweenApp::update() {
	simple_tween.update();
}

void TweenApp::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT){
		
		poPoint val;
		
		if(!zoomed)								//	sets an end value for the scale tween so we can zoom in and then back out
			val = poPoint(2,2,1);				//	all three arguments needed for a scale tween (since z coord defaults to 0)
		else
			val = poPoint(1,1,1);
		zoomed = !zoomed;
		
		// non repeating
		event->source->scaleTween				//	scale tween the object clicked on
		.set(val)								//	scaling is done with a vector
		.setRepeat(PO_TWEEN_REPEAT_NONE)		//	no repeat
		.setTweenFunction(linearFunc)			//	linear (no ease)
		.setDuration(2)							//	Easing functions require a duration
		.start();
	}
}

void TweenApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	if(msg == "done") {
		printf("%s (%f)\n", msg.c_str(), simple_tween_val);
		// reregister to get it again
		poTweenBase *tween = (poTweenBase*)(dict.getPtr("tween"));
		tween->setNotification(this, msg);
	}
}
