//
//  poOpenGLState.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poColor.h"
#include "poMatrixStack.h"

static int maxTextureUnits() {
	static int max = 0;
	if(!max) 
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
	return max;
}

namespace po {
	// http://www.opengl.org/sdk/docs/man/xhtml/glStencilOp.xml
	// http://www.opengl.org/sdk/docs/man/xhtml/glStencilFunc.xml
	struct StencilState {
		StencilState();
		
		bool enabled;
		// stencil operation
		GLenum op_fail;
		GLenum op_stencil_depth_fail;
		GLenum op_stencil_depth_pass;
		// stencil func
		GLenum func;
		GLint func_ref;
		GLuint func_mask;
	};
	
	struct TextureState {
		TextureState();
		
		GLuint bound_id;
	};
	
	// http://www.opengl.org/sdk/docs/man/xhtml/glBlendFunc.xml
	// http://www.opengl.org/sdk/docs/man/xhtml/glBlendEquation.xml
	struct BlendState {
		BlendState();
		
		bool enabled;
		// blend func
		GLenum source_factor, dest_factor;
		// blend equation
		GLenum equation;
		// blend color
		poColor color;
	};
	
}

class poOpenGLState {
public:
	po::StencilState	stencil;
	po::TextureState	texture;
	po::BlendState		blend;
	
	poColor				color;
	poMatrixStack		matrix;
	
	static poOpenGLState *get();
	
	void setStencil(po::StencilState);
	void saveStencil();
	void restoreStencil();

	void setTexture(po::TextureState);
	void saveTexture();
	void restoreTexture();
	
	void setBlend(po::BlendState);
	void saveBlend();
	void restoreBlend();
	
private:
	poOpenGLState();
	
	std::stack<po::StencilState> stencilStack;
	std::stack<po::TextureState> textureStack;
	std::stack<po::BlendState> blendStack;
};




