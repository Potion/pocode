//
//  poOpenGLState.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poOpenGLState.h"

static boost::thread_specific_ptr<poOpenGLState> instance;

// the holder
poOpenGLState::poOpenGLState() 
:	color(poColor::blue)
{}

poOpenGLState *poOpenGLState::get() {
	if(!instance.get())
		instance.reset(new poOpenGLState());
	return instance.get();
}

void poOpenGLState::setStencil(po::StencilState state) {
	if(state.enabled != stencil.enabled) {
		state.enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
		stencil.enabled = state.enabled;
	}
	if(state.op_fail != stencil.op_fail || 
	   state.op_stencil_depth_fail != stencil.op_stencil_depth_fail ||
	   state.op_stencil_depth_pass != stencil.op_stencil_depth_pass) 
	{
		glStencilOp(state.op_fail, state.op_stencil_depth_fail, state.op_stencil_depth_pass);
		stencil.op_fail = state.op_fail;
		stencil.op_stencil_depth_fail = state.op_stencil_depth_fail;
		stencil.op_stencil_depth_pass = state.op_stencil_depth_pass;
	}
	if(state.func != stencil.func ||
	   state.func_ref != stencil.func_ref ||
	   state.func_mask != stencil.func_mask)
	{
		glStencilFunc(state.func, state.func_ref, state.func_mask);
		stencil.func = state.func;
		stencil.func_ref = state.func_ref;
		stencil.func_mask = state.func_mask;
	}
}
void poOpenGLState::saveStencil() {stencilStack.push(stencil);}
void poOpenGLState::restoreStencil() {
	if(!stencilStack.empty()) {
		setStencil(stencilStack.top());
		stencilStack.pop();
	}
	else {
		setStencil(po::StencilState());
	}
}


void poOpenGLState::setTexture(po::TextureState state) {
	if(state.bound_id != texture.bound_id) {
		glBindTexture(GL_TEXTURE_2D, state.bound_id);
		texture.bound_id = state.bound_id;
	}
}
void poOpenGLState::saveTexture() {textureStack.push(texture);}
void poOpenGLState::restoreTexture() {
	if(!textureStack.empty()) {
		setTexture(textureStack.top());
		textureStack.pop();
	}
	else {
		setTexture(po::TextureState());
	}
}


void poOpenGLState::setBlend(po::BlendState state) {
	if(state.enabled != blend.enabled) {
		state.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		blend.enabled = state.enabled;
	}
	if(state.source_factor != blend.source_factor || state.dest_factor != blend.dest_factor) {
		glBlendFunc(state.source_factor, state.dest_factor);
		blend.source_factor = state.source_factor;
		blend.dest_factor = state.dest_factor;
	}
	if(state.equation != blend.equation) {
		glBlendEquation(state.equation);
		blend.equation = state.equation;
	}
	if(state.color != blend.color) {
		glBlendColor(state.color.R, state.color.G, state.color.B, state.color.A);
		blend.color = state.color;
	}
}
void poOpenGLState::saveBlend() {blendStack.push(blend);}
void poOpenGLState::restoreBlend() {
	if(!blendStack.empty()) {
		setBlend(blendStack.top());
		blendStack.pop();
	}
	else {
		setBlend(po::BlendState());
	}
}


// the states
po::StencilState::StencilState() {
	enabled = false;
	op_fail = GL_KEEP;
	op_stencil_depth_fail = GL_KEEP;
	op_stencil_depth_pass = GL_KEEP;
	func = GL_ALWAYS;
	func_ref = 0;
	func_mask = UINT_MAX;
}

po::TextureState::TextureState() {
	bound_id = 0;
}

po::BlendState::BlendState() {
	enabled = false;
	
	source_factor = GL_ONE;
	dest_factor = GL_ZERO;
	
	equation = GL_FUNC_ADD;
	
	color = poColor::transparent;
}


