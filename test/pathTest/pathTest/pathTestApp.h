/*	Created by Joshua Fisher on 11/11/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poPath.h"

class pathTestApp : public poObject {
public:
	pathTestApp();
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	
	poPath path;
};