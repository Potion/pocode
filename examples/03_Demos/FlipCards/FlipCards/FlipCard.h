#pragma once

/*	Created by Jonathan Bobrow on 12/5/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class FlipCard : public poObject
{
public:
	FlipCard();
	virtual ~FlipCard();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    void flip();
    void toggleTexture();
    
    poRectShape* rect;
    bool toggled;
    int side;
};