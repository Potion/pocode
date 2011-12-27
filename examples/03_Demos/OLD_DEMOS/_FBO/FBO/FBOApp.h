/*	Created by Joshua Fisher on 7/16/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poFBO.h"

class FBOGenerator : public poObject {
public:
	FBOGenerator();
	virtual void eventHandler(poEvent* e);
	poFBO *fbo;
};

class FBORenderer : public poObject {
public:
	FBORenderer();
	void update();
	void replaceTexture(poTexture *tex);
	poTexture *next;
};
