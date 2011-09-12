/*	Created by Jonathan Bobrow on 8/23/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class BoundsApp : public poObject {
public:
	BoundsApp();
	virtual ~BoundsApp();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poObject* pop;
	poRectShape	*kid1, *kid2;
};