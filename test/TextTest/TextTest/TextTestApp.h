/*	Created by Joshua Fisher on 8/4/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class TextTestApp : public poObject {
public:
	TextTestApp();
	virtual ~TextTestApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};