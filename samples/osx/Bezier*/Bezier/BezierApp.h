/*	Created by Jonathan Bobrow on 11/14/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class BezierApp : public poObject {
public:
	BezierApp();
	virtual ~BezierApp();
	virtual void update();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	void handleProperties(poOvalShape* handle);
	void lineProperties(poLineShape* line);
	
	poShape2D* curve;		// visible bezier curve
	
	poOvalShape* handle_a;	// point A on the curve
	poOvalShape* handle_b;	// handle B, control point
	poOvalShape* handle_c;	// handle C, control point
	poOvalShape* handle_d;	// point D on the curve
	poOvalShape* handle_e;	// handle E, control point
	poOvalShape* handle_f;	// handle F, control point
	poOvalShape* handle_g;	// point G on the curve
	
	poLineShape* a_b;		// line to control point
	poLineShape* c_e;		// line to control point
	poLineShape* f_g;		// line to control point
	
	bool e_touched;
};