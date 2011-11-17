/*	Created by Jonathan Bobrow on 11/16/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class RotationAxisApp : public poObject {
public:
	RotationAxisApp();
	virtual ~RotationAxisApp();
	
	virtual void update();
	void flip();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

	poRectShape* rect;

};