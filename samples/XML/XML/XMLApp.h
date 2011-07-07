/*	Created by Joshua Fisher on 7/7/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class XMLApp : public poObject {
public:
	XMLApp();
	virtual ~XMLApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};