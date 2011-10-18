/*	Created by Joshua Fisher on 10/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class TextCachingApp : public poObject {
public:
	TextCachingApp();
	virtual ~TextCachingApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};