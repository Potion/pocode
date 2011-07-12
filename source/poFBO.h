//
//  poFBO.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/12/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poTexture.h"

class poFBO {
public:
	poFBO(int width, int height, bool depth, bool stencil, );
	poFBO(int width, int height, bool depth, GLenum );
	poFBO(poTexture *tex, bool depth);
	
	void viewport(int x, int y, int width, int height);
	int width();
	int height();
	
private:
	GLuint uid;
	int width, height;
};