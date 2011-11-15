/*	Created by filippo on 11/15/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class AdvancedShapesApp : public poObject {
public:
	AdvancedShapesApp();
	virtual ~AdvancedShapesApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};