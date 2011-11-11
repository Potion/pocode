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

class poTexture;

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
		GLenum opFail;
		GLenum opStencilDepthFail;
		GLenum opStencilDepthPass;
		// stencil func
		GLenum func;
		GLint funcRef;
		GLuint funcMask;
	};
	
	struct TextureState {
		TextureState();
		TextureState(poTexture *tex);
		
		GLuint boundID;
		bool isMask;
	};
	
	// http://www.opengl.org/sdk/docs/man/xhtml/glBlendFunc.xml
	// http://www.opengl.org/sdk/docs/man/xhtml/glBlendEquation.xml
	struct BlendState {
		BlendState();

		static BlendState preMultipliedBlending();
		
		void blendFunc(GLenum sourceFactor, GLenum destFactor);
		void blendFunc(GLenum sourceFactor, GLenum destFactor, GLenum sourceAlphaFactor, GLenum destAlphaFactor);
		void blendEquation(GLenum mode);
		void blendEquation(GLenum rgbMode, GLenum aMode);
		
		bool isBlendFuncSame(BlendState bs);
		bool isBlendEquationSame(BlendState bs);
		
		void copyBlendFuncFrom(BlendState bs);
		void copyBlendEquationFrom(BlendState bs);
		
		bool enabled;
		// blend func
		GLenum sourceFactor, destFactor, sourceAlphaFactor, destAlphaFactor;
		// blend equation
		GLenum equation, alphaEquation;
		// blend color
		poColor color;
	};
	
	struct VertexState {
		VertexState();
		VertexState &enableAttrib(GLuint);
		VertexState &disableAttrib(GLuint);
		bool isAttribEnabled(GLuint);
		
	private:
		int enabledAttribs;
	};
	
	struct ShaderState {
		ShaderState();
		
		GLuint bound_id;
	};
}

class poOpenGLState {
public:
	po::StencilState stencil;
	po::TextureState texture;
	po::BlendState blend;
	po::VertexState vertex;
	po::ShaderState shader;
	
	poColor				color;
	poMatrixStack		matrix;
	
	static poOpenGLState *get();
	void compactMemory();
	
	void setStencil(po::StencilState, bool forceAccept=false);
	void setTexture(po::TextureState, bool forceAccept=false);
	void setBlend(po::BlendState, bool forceAccept=false);
	void setVertex(po::VertexState, bool forceAccept=false);
	void setShader(po::ShaderState, bool forceAccept=false);
	
	GLint maxVertexAttribs();
	GLint maxFBOSamples();
	GLint maxColorAttachments();

	void pushStencilState();
	void popStencilState();
	
	void pushTextureState();
	void popTextureState();

	void pushBlendState();
	void popBlendState();
	
	void pushVertexState();
	void popVertexState();
	
	void pushShaderState();
	void popShaderState();
	
private:
	poOpenGLState();
	
	int max_vert_attribs, max_fbo_samples, max_color_attachments;
	
	std::stack<po::StencilState> stencilStack;
	std::stack<po::TextureState> textureStack;
	std::stack<po::BlendState> blendStack;
	std::stack<po::VertexState> vertexStack;
	std::stack<po::ShaderState> shaderStack;

	static boost::thread_specific_ptr<poOpenGLState> instance;
};




