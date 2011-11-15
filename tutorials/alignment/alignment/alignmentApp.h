/*	Created by Tamar Ziv on 11/14/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"


class alignmentApp : public poObject {
public:
	alignmentApp();
	virtual ~alignmentApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};