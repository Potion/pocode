/*	Created by Joshua Fisher on 8/9/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

#include "flyingText.h"

class SplitTextApp : public poObject {
public:
	SplitTextApp();

	void eventHandler(poEvent *event);
	
	flyingText* t;
};