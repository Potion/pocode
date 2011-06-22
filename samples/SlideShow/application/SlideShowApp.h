/*	Created by Jonathan Bobrow on 6/22/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"

class SlideShowApp : public poObject {
public:
	SlideShowApp();
	virtual ~SlideShowApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};