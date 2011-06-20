/*	Created by Jonathan Bobrow on 6/20/11.
 *	Copyright 2011 Potion. All rights reserved.
 *
 *
 *  This sample demonstrates the variety of tweens
 *
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class TweenApp : public poObject {
public:
	TweenApp();
	virtual ~TweenApp();
    
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poRectShape* square;

};