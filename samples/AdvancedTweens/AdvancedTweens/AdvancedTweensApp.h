/*	Created by Jonathan Bobrow on 8/12/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class AdvancedTweensApp : public poObject {
public:
	AdvancedTweensApp();
	virtual ~AdvancedTweensApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};