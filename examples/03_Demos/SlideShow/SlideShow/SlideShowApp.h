/*	Created by Jonathan Bobrow on 6/22/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poTextBox.h"

class SlideShowApp : public poObject {
public:
	SlideShowApp();
	virtual void eventHandler(poEvent *event);
	
	poObject *holder;
	int current;

	poTextBox *readout;
};