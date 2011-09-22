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
:	color(poColor::white)
{}

poOpenGLState *poOpenGLState::get() {
	if(!instance.get())
		instance.reset(new poOpenGLState());
	return instance.get();
}

void poOpenGLState::setStencilState(po::StencilState state) {
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

void poOpenGLState::setTextureState(po::TextureState state) {
	if(state.bound_id != texture.bound_id) {
		glBindTexture(GL_TEXTURE_2D, state.bound_id);
		texture.bound_id = state.bound_id;
		
		std::cout << "changed to: " << texture.bound_id << "\n";
	}
}

void poOpenGLState::setShaderState(po::ShaderState state) {
	if(state.bound_id != shader.bound_id) {
		glUseProgram(state.bound_id);
		shader.bound_id = state.bound_id;
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

po::TextureState::TextureState(GLuint uid) {
	bound_id = uid;
}

po::ShaderState::ShaderState() {
	bound_id = 0;
}



