/*	Created by Jonathan Bobrow on 6/21/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

// press 'b' to draw bounds
// press '+' to enlarge type
// press '-' to shrink type

#include "poObject.h"
#include "poTextBox.h"
#include "poFont.h"

class TextBoxApp : public poObject {
public:
	TextBoxApp();
	virtual ~TextBoxApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

	poTextBox *tb, *tb2;
	poFont* f;
};