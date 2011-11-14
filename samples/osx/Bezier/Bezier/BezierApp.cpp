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
	
	// create 7 points (point a + d + g on line, point b + c + e + f as control points)
	handle_a = new poOvalShape( 10, 10, 50);
	handle_b = new poOvalShape( 10, 10, 50);	
	handle_c = new poOvalShape( 10, 10, 50);
	handle_d = new poOvalShape( 10, 10, 50);
	handle_e = new poOvalShape( 10, 10, 50);
	handle_f = new poOvalShape( 10, 10, 50);
	handle_g = new poOvalShape( 10, 10, 50);
	
	// set positions of 7 points
	handle_a->position = poPoint( 100, 150, 0);
	handle_b->position = poPoint( 150, 250, 0);
	handle_c->position = poPoint( 250, 250, 0);
	handle_d->position = poPoint( 300, 150, 0);
	handle_e->position = 2*handle_d->position - handle_c->position;
	handle_f->position = poPoint( 450, 50, 0);
	handle_g->position = poPoint( 500, 150, 0);
	
	// add handle properties
	handleProperties(handle_a);
	handleProperties(handle_b);
	handleProperties(handle_c);
	handleProperties(handle_d);
	handleProperties(handle_e);
	handleProperties(handle_f);
	handleProperties(handle_g);
	
	// create lines to show connection of point to control point
	a_b = new poLineShape( handle_a->position, handle_b->position);
	c_e = new poLineShape( handle_c->position, handle_e->position);
	f_g = new poLineShape( handle_f->position, handle_g->position);
	
	// add line properties
	lineProperties(a_b);
	lineProperties(c_e);
	lineProperties(f_g);
	
	// create curve with the 7 points
	curve = new poShape2D();
	curve->addPoint(handle_a->position);
	curve->curveTo( handle_d->position, handle_b->position, handle_c->position, 100);
	curve->curveTo( handle_g->position, handle_e->position, handle_f->position, 100);
	
	// set properties of the curve
	curve->closed = false;
	curve->fillEnabled = false;
	curve->generateStroke(2);
	curve->useSimpleStroke = true;
	
	// add curve to the screen
	addChild(curve);
	
	e_touched = false;
}

BezierApp::~BezierApp() {
}

void BezierApp::update(){
	
	// update dependent handle
	if( e_touched ){
		handle_c->position = 2*handle_d->position - handle_e->position;
		e_touched = false;
	}
	else
		handle_e->position = 2*handle_d->position - handle_c->position;
	
	// redraw curve to account for changes
	curve->clearPoints();
	curve->addPoint(handle_a->position);
	curve->curveTo( handle_d->position, handle_b->position, handle_c->position, 100);
	curve->curveTo( handle_g->position, handle_e->position, handle_f->position, 100);
	curve->generateStroke(2);
	
	// reshape lines to account for changes
	a_b->reshape(handle_a->position, handle_b->position);
	c_e->reshape(handle_c->position, handle_e->position);
	f_g->reshape(handle_f->position, handle_g->position);
	
	a_b->generateStroke(2);
	c_e->generateStroke(2);
	f_g->generateStroke(2);
	
}

void BezierApp::eventHandler(poEvent *event) {
	
	if( event->type == PO_MOUSE_DRAG_INSIDE_EVENT || event->type == PO_MOUSE_DOWN_INSIDE_EVENT){
		
		// move point with mouse when down or dragging
		event->source->position = event->globalPosition;
		
		if ( event->source == handle_e )
			e_touched = true;
	}
	
}

void BezierApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}

void BezierApp::handleProperties(poOvalShape *handle){
	
	handle->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
	handle->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	addChild(handle);
	
}

void BezierApp::lineProperties(poLineShape *line){
	
	line->strokeColor = poColor::magenta;
	line->generateStroke(2);
	line->useSimpleStroke = true;
	line->alpha = .2f;
	addChild(line);
}