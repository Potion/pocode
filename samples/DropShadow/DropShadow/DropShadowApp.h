/*	Created by Jonathan Bobrow on 8/26/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poDropShadow.h"

class DropShadowApp : public poObject {
public:
	DropShadowApp();
	virtual ~DropShadowApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* item;
	poDropShadow* shadow;
};