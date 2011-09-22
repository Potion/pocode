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
		TextureState(GLuint uid);
		
		GLuint bound_id;
	};
	
	struct ShaderState {
		ShaderState();
		
		GLuint bound_id;
	};
}

class poOpenGLState {
public:
	po::StencilState	stencil;
	po::TextureState	texture;
	po::ShaderState		shader;
	
	poColor				color;
	poMatrixStack		matrix;
	
	static poOpenGLState *get();
	void setStencilState(po::StencilState);
	void setTextureState(po::TextureState);
	void setShaderState(po::ShaderState);
	
private:
	poOpenGLState();
};




