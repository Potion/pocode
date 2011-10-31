#include "BezierApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BezierApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Bezier", 100, 100, 600, 300);
}

void cleanupApplication() {
}

BezierApp::BezierApp() {
	addModifier(new poCamera2D(poColor::black));
	
	// create 4 points (point a + d on line, point b + c as control points)
	handle_a = new poOvalShape( 10, 10, 50);
	handle_b = new poOvalShape( 10, 10, 50);	
	handle_c = new poOvalShape( 10, 10, 50);
	handle_d = new poOvalShape( 10, 10, 50);
	
	handle_a->position = poPoint( 150, 250, 0);
	handle_b->position = poPoint( 200, 50, 0);
	handle_c->position = poPoint( 400, 50, 0);
	handle_d->position = poPoint( 450, 250, 0);
	
	// add events to allow for moving points
	handle_a->addEvent(PO_MOUSE_DRAG_EVENT, this);
	handle_b->addEvent(PO_MOUSE_DRAG_EVENT, this);
	handle_c->addEvent(PO_MOUSE_DRAG_EVENT, this);
	handle_d->addEvent(PO_MOUSE_DRAG_EVENT, this);
	handle_a->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	handle_b->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	handle_c->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	handle_d->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	
	// create lines to show connection of point to control point
	a_b = new poLineShape( handle_a->position, handle_b->position);
	c_d = new poLineShape( handle_c->position, handle_d->position);
	a_b->strokeColor = poColor::magenta;
	c_d->strokeColor = poColor::magenta;
	a_b->generateStroke(2);
	c_d->generateStroke(2);
	a_b->useSimpleStroke = true;
	c_d->useSimpleStroke = true;
	a_b->alpha = .2f;
	c_d->alpha = .2f;
	
	// create curve with the 4 points
	curve = new poShape2D();
	curve->addPoint(handle_a->position);
	curve->curveTo( handle_d->position, handle_b->position, handle_c->position, 100);
	
	// set properties of the curve
	curve->closed = false;
	curve->fillEnabled = false;
	curve->generateStroke(2);
	curve->useSimpleStroke = true;

	// add points, lines, and curve to the screen
	addChild(handle_a);
	addChild(handle_b);
	addChild(handle_c);
	addChild(handle_d);
	addChild(a_b);
	addChild(c_d);
	addChild(curve);
}

BezierApp::~BezierApp() {
}

void BezierApp::update(){
	
	// redraw curve to account for changes
	curve->clearPoints();
	curve->addPoint(handle_a->position);
	curve->curveTo( handle_d->position, handle_b->position, handle_c->position, 100);
	curve->generateStroke(2);
	
	// reshape lines to account for changes
	a_b->reshape(handle_a->position, handle_b->position);
	c_d->reshape(handle_c->position, handle_d->position);
	a_b->generateStroke(2);
	c_d->generateStroke(2);
	
}

void BezierApp::eventHandler(poEvent *event) {
	
	if( event->type == PO_MOUSE_DRAG_EVENT || event->type == PO_MOUSE_DOWN_INSIDE_EVENT){
		
		// move point with mouse when down or dragging
		event->source->position = event->position;
	}
	
}

void BezierApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
