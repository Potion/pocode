#pragma once

/*	Created by Joshua Fisher on 7/16/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poTexture.h"
#include "poObjectModifier.h"

class poFBO : public poObjectModifier
{
public:
	poFBO(uint w, uint h);
	virtual ~poFBO();
	
	void resize(uint w, uint h);
	
	bool isValid() const;
	poTexture *texture() const;

protected:
	virtual void doSetUp(poObject*);
	virtual void doSetDown(poObject*);

private:
	GLuint uid;
	poTexture *tex;
};