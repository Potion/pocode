/*	Created by Joshua Fisher on 10/17/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poFBO;

class FBO_TestApp : public poObject {
public:
	FBO_TestApp();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poFBO *fbo;
	bool fancyDraw;
};