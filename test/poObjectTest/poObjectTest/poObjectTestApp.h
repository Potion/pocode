/*	Created by Joshua Fisher on 8/8/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poObjectTestApp : public poObject {
public:
	poObjectTestApp();
	virtual ~poObjectTestApp();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};