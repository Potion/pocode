/*	Created by Joshua Fisher on 7/25/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class AlphaTestTextureApp : public poObject {
public:
	AlphaTestTextureApp();
	virtual ~AlphaTestTextureApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};