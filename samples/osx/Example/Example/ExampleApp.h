/*	Created by Joshua Fisher on 7/26/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poShape2D;

class ExampleApp : public poObject {
public:
	ExampleApp();
	virtual ~ExampleApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poShape2D *mask;
};