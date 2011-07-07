/*	Created by Joshua Fisher on 7/7/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class TextAlignmentTestApp : public poObject {
public:
	TextAlignmentTestApp();
	virtual ~TextAlignmentTestApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};