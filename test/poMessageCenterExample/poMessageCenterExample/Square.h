#pragma once

/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

static const std::string ToggleSquareMessage = "TOGGLE_SQUARE_MESSAGE";

class Square : public poObject
{
public:
	Square();
	virtual ~Square();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poRectShape* shape;
    
    bool bToggle;
};