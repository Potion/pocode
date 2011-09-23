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
	save();
}

void poOpenGLStateChange::doSetDown(poObject *obj) {
	restore();
}

void poStencilState::save() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->saveStencil();
	ogl->setStencil(state);
}

void poStencilState::restore() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->restoreStencil();
}

poTextureState::poTextureState() {
	state.bound_id = 0;
}

poTextureState::poTextureState(GLuint uid, bool is_mask) {
	state.bound_id = uid;
	state.is_mask = is_mask;
}

void poTextureState::save() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->saveTexture();
	ogl->setTexture(state);
}

void poTextureState::restore() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->restoreTexture();
}

poBlendState::poBlendState() {}

poBlendState::poBlendState(GLenum sfactor, GLenum dfactor) {
	state.enabled = true;
	state.source_factor = sfactor;
	state.dest_factor = dfactor;
}

void poBlendState::save() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->saveBlend();
	ogl->setBlend(state);
}

void poBlendState::restore() {
	poOpenGLState* ogl = poOpenGLState::get();
	ogl->restoreBlend();
}



