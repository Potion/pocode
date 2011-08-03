/*	Created by Jonathan Bobrow on 6/21/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

// press 'b' to draw bounds
// press '+' to enlarge type
// press '-' to shrink type

#include "poObject.h"
#include "poTextBox.h"

class TextBoxApp : public poObject {
public:
	TextBoxApp();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);

	poTextBox *tb, *tb2;
};