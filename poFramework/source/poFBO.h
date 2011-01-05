/*
 *  poFBO.h
 *  poFramework3
 *
 *  Created by Joshua Fisher on 3/24/08.
 *  Copyright 2008 Potion Design, LLC. All rights reserved.
 *
 */

#include "poCommon.h"
#include "poObjectModifier.h"

#ifndef _PO_FBO_H_
#define _PO_FBO_H_

class poObject;
class poTexture;

class poFBO : public poObjectModifier
{
public:
	poFBO(float w, float h);
	~poFBO();
	
	virtual void			init() { };
	virtual const char*		getClassName() { return "poFBO"; };
	
	virtual void setup(poObject*);
	virtual void setdown(poObject*);
	
	float width, height;
	
	GLuint fboID, depthbufferID;
	poTexture* tex;
};

#endif