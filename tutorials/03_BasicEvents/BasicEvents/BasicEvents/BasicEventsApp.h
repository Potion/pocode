/*	Created by Jonathan Bobrow on 11/14/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicEventsApp : public poObject {
public:
	BasicEventsApp();
	virtual ~BasicEventsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* rectangle;
};