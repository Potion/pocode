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
poOpenGLState::poOpenGLState() : color(poColor::white) {
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

void poOpenGLState::setStencil(po::StencilState state, bool forceAccept) {
	if(forceAccept) {
		state.enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
		glStencilOp(state.opFail, state.opStencilDepthFail, state.opStencilDepthPass);
		glStencilFunc(state.func, state.funcRef, state.funcMask);
		stencil = state;
	}
	else {
		if(state.enabled != stencil.enabled) {
			state.enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			stencil.enabled = state.enabled;
		}
		if(state.opFail != stencil.opFail || 
		   state.opStencilDepthFail != stencil.opStencilDepthFail ||
		   state.opStencilDepthPass != stencil.opStencilDepthPass)
		{
			glStencilOp(state.opFail, state.opStencilDepthFail, state.opStencilDepthPass);
			stencil.opFail = state.opFail;
			stencil.opStencilDepthFail = state.opStencilDepthFail;
			stencil.opStencilDepthPass = state.opStencilDepthPass;
		}
		if(state.func != stencil.func ||
		   state.funcRef != stencil.funcRef ||
		   state.funcMask != stencil.funcMask)
		{
			glStencilFunc(state.func, state.funcRef, state.funcMask);
			stencil.func = state.func;
			stencil.funcRef = state.funcRef;
			stencil.funcMask = state.funcMask;
		}
	}
}

void poOpenGLState::setTexture(po::TextureState state, bool forceAccept) {
	if(forceAccept) {
		glBindTexture(GL_TEXTURE_2D, state.boundID);
		texture = state;
	}
	else {
		if(state.boundID != texture.boundID) {
			glBindTexture(GL_TEXTURE_2D, state.boundID);
			texture.boundID = state.boundID;
		}
		texture.isMask = state.isMask;
	}
}

void poOpenGLState::setBlend(po::BlendState state, bool forceAccept) {
	if(forceAccept) {
		state.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		glBlendFuncSeparate(state.sourceFactor, state.destFactor, state.sourceAlphaFactor, state.destAlphaFactor);
		glBlendEquationSeparate(state.equation, state.alphaEquation);
		glBlendColor(state.color.R, state.color.G, state.color.B, state.color.A);
		blend = state;
	}
	else {
		if(state.enabled != blend.enabled) {
			state.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			blend.enabled = state.enabled;
		}
		
		if(!blend.isBlendFuncSame(state)) {
			glBlendFuncSeparate(state.sourceFactor, state.destFactor, state.sourceAlphaFactor, state.destAlphaFactor);
			blend.copyBlendFuncFrom(state);
		}
		
		if(!blend.isBlendEquationSame(state)) {
			glBlendEquationSeparate(state.equation, state.alphaEquation);
			blend.copyBlendEquationFrom(state);
		}

		if(state.color != blend.color) {
			glBlendColor(state.color.R, state.color.G, state.color.B, state.color.A);
			blend.color = state.color;
		}
	}
}

void poOpenGLState::setVertex(po::VertexState vert, bool forceAccept) {
	if(forceAccept) {
		for(int i=0; i<maxVertexAttribs(); i++) {
			if(vert.isAttribEnabled(i))
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
			vertex = vert;
		}
	}
	else {
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
}

void poOpenGLState::setShader(po::ShaderState sh, bool forceAccept) {
	if(forceAccept) {
		glUseProgram(sh.bound_id);
		shader = sh;
	}
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
	opFail = GL_KEEP;
	opStencilDepthFail = GL_KEEP;
	opStencilDepthPass = GL_KEEP;
	func = GL_ALWAYS;
	funcRef = 0;
	funcMask = UINT_MAX;
}

po::BlendState::BlendState() {
	enabled = false;
	
	blendEquation(GL_FUNC_ADD);
	blendFunc(GL_ONE, GL_ZERO);
	
	color = poColor::transparent;
}

po::BlendState po::BlendState::preMultipliedBlending() {
	BlendState state;
	state.enabled =true;
	state.blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return state;
}

void po::BlendState::blendFunc(GLenum sourceFactor, GLenum destFactor) {
	this->sourceFactor = sourceAlphaFactor = sourceFactor;
	this->destFactor = destAlphaFactor = destFactor;
}

void po::BlendState::blendFunc(GLenum sourceFactor, GLenum destFactor, GLenum sourceAlphaFactor, GLenum destAlphaFactor) {
	this->sourceFactor = sourceFactor;
	this->destFactor = destFactor;
	this->sourceAlphaFactor = sourceAlphaFactor;
	this->destAlphaFactor = destAlphaFactor;
}

void po::BlendState::blendEquation(GLenum mode) {
	equation = alphaEquation = mode;
}

void po::BlendState::blendEquation(GLenum rgbMode, GLenum aMode) {
	equation = rgbMode;
	alphaEquation = aMode;
}

bool po::BlendState::isBlendFuncSame(po::BlendState bs) {
	return (sourceFactor == bs.sourceFactor &&
			destFactor == bs.destFactor &&
			sourceAlphaFactor == bs.sourceAlphaFactor &&
			destAlphaFactor == bs.destAlphaFactor);
}

bool po::BlendState::isBlendEquationSame(po::BlendState bs) {
	return (equation == bs.equation && alphaEquation == bs.alphaEquation);
}

void po::BlendState::copyBlendFuncFrom(po::BlendState bs) {
	sourceFactor = bs.sourceFactor;
	destFactor = bs.destFactor;
	sourceAlphaFactor = bs.sourceAlphaFactor;
	destAlphaFactor = bs.destAlphaFactor;
}

void po::BlendState::copyBlendEquationFrom(po::BlendState bs) {
	equation = bs.equation;
	alphaEquation = bs.alphaEquation;
}


po::TextureState::TextureState() {
	boundID = 0;
	isMask = false;
}

po::TextureState::TextureState(poTexture tex) {
	boundID = tex.getUid();
	isMask = tex.getConfig().format == GL_ALPHA;
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

