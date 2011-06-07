/*	Created by Jonathan Bobrow on 6/6/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

using namespace std;

class MouseEventsApp : public poObject {
public:
	MouseEventsApp();
	virtual ~MouseEventsApp();
    virtual void eventHandler(poEvent *event);
    
    poRectShape* rectangle;
};