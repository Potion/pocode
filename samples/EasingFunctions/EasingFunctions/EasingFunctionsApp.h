/*	Created by Jonathan Bobrow on 8/12/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class EasingFunctionsApp : public poObject {
public:
	EasingFunctionsApp();
	virtual ~EasingFunctionsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poOvalShape* s;
};