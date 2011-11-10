/*	Created by Jonathan Bobrow on 10/30/11.
 *	Copyright 2011 Potion. All rights reserved.
 *
 *	Simple Bezier Curve visualizer, helps show how the curve functions can be used.
 *	This sample shows a cubic bezier curve, since it recieves two control points.
 *
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

	poShape2D* curve;		// visible bezier curve
	
	poOvalShape* handle_a;	// point A on the curve
	poOvalShape* handle_b;	// handle B, control point
	poOvalShape* handle_c;	// handle C, control point
	poOvalShape* handle_d;	// point D on the curve
	
	poLineShape* a_b;		// line to control point
	poLineShape* c_d;		// line to control point
};