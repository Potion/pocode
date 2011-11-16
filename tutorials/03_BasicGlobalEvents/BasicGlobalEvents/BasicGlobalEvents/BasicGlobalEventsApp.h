/*	Created by Potion on 11/15/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicGlobalEventsApp : public poObject {
public:
	BasicGlobalEventsApp();
	virtual ~BasicGlobalEventsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* A;
	poRectShape* B;
	poRectShape* C;
	poRectShape* D;
};