/*	Created by Jonathan Bobrow on 9/7/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "KinectTouch.h"
#include "poTextBox.h"

class TouchEventsApp : public poObject {
public:
	TouchEventsApp();
	virtual ~TouchEventsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* rectangle;
	poOvalShape* marker;
	poTextBox* eventTB;
	KinectTouch* kinect;
	
};