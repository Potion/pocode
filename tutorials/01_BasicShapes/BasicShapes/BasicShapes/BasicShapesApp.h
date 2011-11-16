/*	Created by Potion on 11/14/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"

class BasicShapesApp : public poObject {
public:
	BasicShapesApp();
	virtual ~BasicShapesApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};