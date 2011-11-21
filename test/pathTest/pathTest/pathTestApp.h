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
	
	void addPoint(poPoint p);
	void movePoint(poPoint p);
	void moveControl(poPoint p);
	
	std::vector<poPath> paths;
	poTexture *controlHandle, *pointHandle;
};