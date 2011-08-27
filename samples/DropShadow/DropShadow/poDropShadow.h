#pragma once

/*	Created by Jonathan Bobrow on 7/27/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class poDropShadow : public poObject
{
public:
	poDropShadow( poPoint dimensions, float spread, float alpha);
	virtual ~poDropShadow();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	void setAlpha( float alpha );
	void setSpread( poPoint dimensions, float spread );
	
	poRectShape* top_left;
	poRectShape* top_center;
	poRectShape* top_right;
	poRectShape* center_left;
	poRectShape* center_center;
	poRectShape* center_right;
	poRectShape* bottom_left;
	poRectShape* bottom_center;
	poRectShape* bottom_right;
	
	poPoint* dimensions;
	float spread; 
};