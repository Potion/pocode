//
//  poOpenGLStateChange.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poOpenGLState.h"
#include "poObjectModifier.h"

class poOpenGLStateChange : public poObjectModifier {
public:
	virtual void doSetUp(poObject *obj);
	virtual void doSetDown(poObject *obj);
	virtual void save() = 0;
	virtual void restore() = 0;
};

class poStencilState : public poOpenGLStateChange {
public:
	po::StencilState state;
	
	virtual void save();
	virtual void restore();
};

class poTextureState : public poOpenGLStateChange {
public:
	po::TextureState state;
	
	poTextureState();
	poTextureState(GLuint uid);
	
	virtual void save();
	virtual void restore();
};

class poBlendState : public poOpenGLStateChange {
public:
	po::BlendState state;
	
	poBlendState();
	poBlendState(GLenum sfactor, GLenum dfactor);
	
	virtual void save();
	virtual void restore();
};




