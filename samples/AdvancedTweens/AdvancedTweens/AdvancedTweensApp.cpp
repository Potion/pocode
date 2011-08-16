#include "AdvancedTweensApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedTweensApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedTweens", 100, 100, 600, 300);
}

void cleanupApplication() {
}

AdvancedTweensApp::AdvancedTweensApp()
:float_tween_x(&x_tween_val),
float_tween_y(&y_tween_val)
{
	addModifier(new poCamera2D(poColor::black));
	
	animate();
	
	shape = new poOvalShape(10,10,50);
	addChild(shape);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

AdvancedTweensApp::~AdvancedTweensApp() {
}

void AdvancedTweensApp::update(){
	
	float_tween_y.update();
	float_tween_x.update();
	
	shape->position = poPoint(50+500.f * x_tween_val, 25+250.f * y_tween_val);
}

void AdvancedTweensApp::animate(){
	
	float_tween_x
	.set(1.f)
	.setDuration(1.f)
	.setTweenFunction(linearFunc)
	.setRepeat(PO_TWEEN_REPEAT_PINGPONG)
	.start();

	float_tween_y
	.set(1.f)
	.setDuration(1.f)
	.setTweenFunction(quadInOutFunc)
	.setRepeat(PO_TWEEN_REPEAT_PINGPONG)
	.start();
}

void AdvancedTweensApp::eventHandler(poEvent *event) {
	
	if( event->type == PO_KEY_DOWN_EVENT ){
		x_tween_val = 0.f;
		y_tween_val = 0.f;
		animate();
	}
}

void AdvancedTweensApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
