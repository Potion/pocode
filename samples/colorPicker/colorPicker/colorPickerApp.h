/*	Created by Xiaoyang Feng on 7/16/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poShape2D.h"
#include "poTextBox.h"

class colorPickerApp : public poObject {
public:
	colorPickerApp();
	virtual ~colorPickerApp();
	
	virtual void eventHandler(poEvent *event);

	poImage		*image;
	poShape2D	*indicator;
	poTextBox	*readout;
};