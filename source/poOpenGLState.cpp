//
//  poOpenGLState.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poOpenGLState.h"
#include "poTexture.h"

boost::thread_specific_ptr<poOpenGLState> poOpenGLState::instance;

// the holder
poOpenGLState::poOpenGLState() 
:	color(poColor::white)
{
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vert_attribs);
#ifdef OPENGL_ES
	max_color_attachments = 1;
	#ifdef POTION_IOS
		glGetIntegerv(GL_MAX_SAMPLES_APPLE, &max_fbo_samples);
	#else
		max_fbo_samples = 4;
	#endif
#else
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
	glGetIntegerv(GL_MAX_SAMPLES, &max_fbo_samples);
#endif
}

poOpenGLState *poOpenGLState::get() {
	#if defined(POTION_IOS)
		static poOpenGLState *instance = NULL;
		if(!instance)
			instance = new poOpenGLState();
		return instance;
	#else
		poOpenGLState *state = instance.get();
		if(!state) {
			state = new poOpenGLState();
			instance.reset(state);
		}
		return state;
	#endif
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

void poOpenGLState::setTexture(po::TextureState state) {
	if(state.bound_id != texture.bound_id) {
		glBindTexture(GL_TEXTURE_2D, state.bound_id);
		texture.bound_id = state.bound_id;
	}
	texture.is_mask = state.is_mask;
}

void poOpenGLState::setBlend(po::BlendState state) {
	if(state.enabled != blend.enabled) {
		state.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		blend.enabled = state.enabled;
	}
	
	if(!blend.isBlendFuncSame(blend)) {
		glBlendFuncSeparate(state.source_factor, state.dest_factor, state.source_alpha_factor, state.dest_alpha_factor);
		blend.copyBlendFuncFrom(state);
	}
	
	if(!blend.isBlendEquationSame(state)) {
		glBlendEquationSeparate(state.equation, state.alpha_equation);
		blend.copyBlendEquationFrom(state);
	}

	if(state.color != blend.color) {
		glBlendColor(state.color.R, state.color.G, state.color.B, state.color.A);
		blend.color = state.color;
	}
}

void poOpenGLState::setVertex(po::VertexState vert) {
	for(int i=0; i<maxVertexAttribs(); i++) {
		bool shouldEnable = vert.isAttribEnabled(i);
		bool isEnabled = vertex.isAttribEnabled(i);
		if(shouldEnable && !isEnabled) {
			vertex.enableAttrib(i);
			glEnableVertexAttribArray(i);
		}
		else 
		if(!shouldEnable && isEnabled) {
			vertex.disableAttrib(i);
			glDisableVertexAttribArray(i);
		}
	}
}

void poOpenGLState::setShader(po::ShaderState sh) {
	if(shader.bound_id != sh.bound_id) {
		shader.bound_id = sh.bound_id;
		glUseProgram(shader.bound_id);
	}
}

GLint poOpenGLState::maxVertexAttribs() {
	return max_vert_attribs;
}

GLint poOpenGLState::maxFBOSamples() {
	return max_fbo_samples;
}

GLint poOpenGLState::maxColorAttachments() {
	return max_color_attachments;
}

void poOpenGLState::pushStencilState() {
	stencilStack.push(stencil);
}
void poOpenGLState::popStencilState() {
	setStencil(stencilStack.top());
	stencilStack.pop();
}

void poOpenGLState::pushTextureState() {
	textureStack.push(texture);
}
void poOpenGLState::popTextureState() {
	setTexture(textureStack.top());
	textureStack.pop();
}

void poOpenGLState::pushBlendState() {
	blendStack.push(blend);
}
void poOpenGLState::popBlendState() {
	setBlend(blendStack.top());
	blendStack.pop();
}

void poOpenGLState::pushVertexState() {
	vertexStack.push(vertex);
}
void poOpenGLState::popVertexState() {
	setVertex(vertexStack.top());
	vertexStack.pop();
}

void poOpenGLState::pushShaderState() {
	shaderStack.push(shader);
}

void poOpenGLState::popShaderState() {
	setShader(shaderStack.top());
	shaderStack.pop();
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

po::BlendState::BlendState() {
	enabled = false;
	
	blendEquation(GL_FUNC_ADD);
	blendFunc(GL_ONE, GL_ZERO);
	
	color = poColor::transparent;
}

po::BlendState po::BlendState::defaultBlending() {
	BlendState state;
	state.enabled =true;
	state.blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return state;
}

void po::BlendState::blendFunc(GLenum sourceFactor, GLenum destFactor) {
	source_factor = source_alpha_factor = sourceFactor;
	dest_factor = dest_alpha_factor = destFactor;
}
void po::BlendState::blendFunc(GLenum sourceFactor, GLenum destFactor, GLenum sourceAlphaFactor, GLenum destAlphaFactor) {
	source_factor = sourceFactor;
	dest_factor = destFactor;
	source_alpha_factor = sourceAlphaFactor;
	dest_alpha_factor = destAlphaFactor;
}
void po::BlendState::blendEquation(GLenum mode) {
	equation = alpha_equation = mode;
}
void po::BlendState::blendEquation(GLenum rgbMode, GLenum aMode) {
	equation = rgbMode;
	alpha_equation = aMode;
}
bool po::BlendState::isBlendFuncSame(po::BlendState bs) {
	return (source_factor == bs.source_factor &&
			dest_factor == bs.dest_factor &&
			source_alpha_factor == bs.source_alpha_factor &&
			dest_alpha_factor == bs.dest_alpha_factor);
}
bool po::BlendState::isBlendEquationSame(po::BlendState bs) {
	return (equation == bs.equation && alpha_equation == bs.alpha_equation);
}
void po::BlendState::copyBlendFuncFrom(po::BlendState bs) {
	source_factor = bs.source_factor;
	dest_factor = bs.dest_factor;
	source_alpha_factor = bs.source_alpha_factor;
	dest_alpha_factor = bs.dest_alpha_factor;
}
void po::BlendState::copyBlendEquationFrom(po::BlendState bs) {
	equation = bs.equation;
	alpha_equation = bs.alpha_equation;
}


po::TextureState::TextureState() {
	bound_id = 0;
	is_mask = false;
}

po::TextureState::TextureState(poTexture tex) {
	bound_id = tex.getUid();
	is_mask = tex.getConfig().format == GL_ALPHA;
}

po::VertexState::VertexState() 
:	enabledAttribs(0)
{}
po::VertexState & po::VertexState::enableAttrib(GLuint i) {
	enabledAttribs |= (1 << i);
	return *this;
}
po::VertexState & po::VertexState::disableAttrib(GLuint i) {
	enabledAttribs &= ~(1 << i);
	return *this;
}
bool po::VertexState::isAttribEnabled(GLuint i) {
	return (enabledAttribs & (1 << i)) > 0;
}

po::ShaderState::ShaderState()
:	bound_id(0)
{}

