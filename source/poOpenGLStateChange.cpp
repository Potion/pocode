//
//  poOpenGLStateChange.cpp
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poOpenGLStateChange.h"
#include "poObject.h"

void poOpenGLStateChange::doSetUp(poObject *obj) {
	set();
}

void poOpenGLStateChange::doSetDown(poObject *obj) {
	revert();
}

void poStencilState::set() {
	saved = poOpenGLState::get()->stencil;
	poOpenGLState::get()->setStencilState(state);
}

void poStencilState::revert() {
	poOpenGLState::get()->setStencilState(saved);
}

poTextureState::poTextureState() {
	state.bound_id = 0;
	set();
}

poTextureState::poTextureState(GLuint uid) {
	state.bound_id = uid;
	set();
}

poTextureState::~poTextureState() {
	revert();
}

void poTextureState::set() {
	saved = poOpenGLState::get()->texture;
	poOpenGLState::get()->setTextureState(state);
}

void poTextureState::revert() {
	poOpenGLState::get()->setTextureState(saved);
}

void poShaderState::set() {
	saved = poOpenGLState::get()->shader;
	poOpenGLState::get()->setShaderState(state);
}

void poShaderState::revert() {
	poOpenGLState::get()->setShaderState(saved);
}

