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
	
	// a shape to move on screen
	shape = new poOvalShape(10,10,50);
	addChild(shape);
	
	// a function to set the tween properties
	animate();
	
	// a little key control
	addEvent(PO_KEY_DOWN_EVENT, this);
}

AdvancedTweensApp::~AdvancedTweensApp() {
}

void AdvancedTweensApp::update(){
	
	// update our tween values every frame
	float_tween_y.update();
	float_tween_x.update();
	
	// and the position of our shape
	shape->position = poPoint(50+500.f * x_tween_val, 25+250.f * y_tween_val);
	// and for fun, the scale of the shape based on y value
	shape->scale = poPoint( 5-8*fabs(y_tween_val-.5f), 5-8*fabs(y_tween_val-.5f), 1.f); 
}

void AdvancedTweensApp::animate(){
	
	
	// set the tween values
	float_tween_x
	.set(1.f)
	.setDuration(1.f)
	.setTweenFunction(linearFunc)
	.start();

	float_tween_y
	.set(1.f)
	.setDuration(1.f)
	.setTweenFunction(quadInOutFunc)
	.start();
}

void AdvancedTweensApp::eventHandler(poEvent *event) {
	
	// reset the tween values
	if( event->type == PO_KEY_DOWN_EVENT ){
		x_tween_val = 0.f;
		y_tween_val = 0.f;
		animate();
	}
}

void AdvancedTweensApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
