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
	virtual void set() = 0;
	virtual void revert() = 0;
};

class poStencilState : public poOpenGLStateChange {
public:
	po::StencilState state;
	
	virtual void set();
	virtual void revert();
	
private:
	po::StencilState saved;
};

class poTextureState : public poOpenGLStateChange {
public:
	po::TextureState state;
	
	poTextureState();
	poTextureState(GLuint uid);
	~poTextureState();
	
	virtual void set();
	virtual void revert();
	
private:
	po::TextureState saved;
};

class poShaderState : public poOpenGLStateChange {
public:
	po::ShaderState state;
	
	virtual void set();
	virtual void revert();
	
private:
	po::ShaderState saved;
};
