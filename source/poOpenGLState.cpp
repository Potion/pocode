/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  poOpenGLState.cpp
//  pocode
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poOpenGLState.h"
#include "poShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <boost/algorithm/string.hpp>

namespace {

	const char * shader_2d =
	"	[[uniforms]]							\n"
	"	uniform mat4 mvp;						\n"
	"	uniform vec4 color;						\n"
	
	"	[[varyings]]							\n"
	
	"	[[vertex]]								\n"
	"	attribute vec4 position;				\n"
	
	"	void main() {							\n"
	"		gl_Position = mvp * position;		\n"
	"	}										\n"
	
	"	[[fragment]]							\n"
	"	void main() {							\n"
	"		gl_FragColor = color;				\n"
	"	}										\n";
	
	const char * shader_3d =
	"	[[uniforms]]							\n"
	"	uniform mat4 modelView;                 \n"
	"   uniform mat4 projection;                \n"
	"	uniform vec4 color;						\n"
	"   uniform vec3 lightPos;                  \n"
	
	"	[[varyings]]							\n"
	"   varying vec3 normalVar;                 \n"
	"   varying vec3 lightDir;                  \n"
	
	"	[[vertex]]								\n"
	"	attribute vec4 position;				\n"
	"   attribute vec3 normal;                  \n"
	
	"	void main() {							\n"
	"       normalVar = (modelView * vec4(normal,0.0)).xyz; \n"
	"       vec4 pos = modelView * position;    \n"
	"       lightDir = lightPos - pos.xyz;		\n"
	"		gl_Position = projection * pos;     \n"
	"	}										\n"
	
	"	[[fragment]]							\n"
	"	void main() {							\n"
	"       float NdotL = max(dot(normalVar,normalize(lightDir)),0.0); \n"
	"		gl_FragColor = color * NdotL;       \n"
	"	}										\n";
	
	const char * shader_tex =
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        "   precision mediump sampler2D;            \n"
    #endif
	"	[[uniforms]]							\n"
	"	uniform mat4 mvp;						\n"
	"	uniform vec4 color;						\n"
	"	uniform sampler2D tex;					\n"
	
	"	[[varyings]]							\n"
	"	varying vec2 texCoord;					\n"
	
	"	[[vertex]]								\n"
	"	attribute vec4 position;				\n"
	"	attribute vec2 textureCoordinates;		\n"
	
	"	void main() {							\n"
	"		texCoord = textureCoordinates;		\n"
	"		gl_Position = mvp * position;		\n"
	"	}										\n"
	
	"	[[fragment]]							\n"
	"	void main() {							\n"
	"		vec4 texColor = texture2D(tex, texCoord);\n"
	"		gl_FragColor = texColor * color;	\n"
	"	}										\n";

	const char * shader_tex_mask =
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	"   precision mediump sampler2D;            \n"
#endif
	"	[[uniforms]]							\n"
	"	uniform mat4 mvp;						\n"
	"	uniform vec4 color;						\n"
	"	uniform sampler2D tex;					\n"
	
	"	[[varyings]]							\n"
	"	varying vec2 texCoord;					\n"
	
	"	[[vertex]]								\n"
	"	attribute vec4 position;				\n"
	"	attribute vec2 textureCoordinates;		\n"
	
	"	void main() {							\n"
	"		texCoord = textureCoordinates;		\n"
	"		gl_Position = mvp * position;		\n"
	"	}										\n"
	
	"	[[fragment]]							\n"
	"	void main() {							\n"
	"		vec4 texColor = texture2D(tex, texCoord);\n"
	"		gl_FragColor = vec4(1.0,1.0,1.0,texColor.a) * color;	\n"
	"	}										\n";

	using namespace glm;

	struct TextureState {
		TextureState()
		:	target(GL_TEXTURE_2D)
		,	bound(0), unit(0), hasAlpha(0)
		{}
		
		int target;
		int bound;
		int unit;
		bool hasAlpha;
	};
	
	struct BlendState {
		BlendState()
		:	enabled(false)
		{
			setEq(GL_FUNC_ADD);
			setFunc(GL_ONE, GL_ZERO);
		}
		
		void setEq(GLenum eq) {
			rgbEq = alphaEq = eq;
		}
		
		void setFunc(GLenum src, GLenum dst) {
			srcRgbFactor = srcAlphaFactor = src;
			dstRgbFactor = dstAlphaFactor = dst;
		}
		
		void apply() {
			enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			glBlendEquationSeparate(rgbEq, alphaEq);
			glBlendFuncSeparate(srcRgbFactor, dstRgbFactor, srcAlphaFactor, dstAlphaFactor);
		}
		
		bool enabled;
		int rgbEq, alphaEq;
		int srcRgbFactor, srcAlphaFactor;
		int dstRgbFactor, dstAlphaFactor;
	};

	struct OpenGLState {
		
		OpenGLState() {
			shader2D.loadSource(shader_2d);
			shader2D.compile();
			glBindAttribLocation(shader2D.getUid(), 0, "position");
			shader2D.link();
			
			shader3D.loadSource(shader_3d);
			shader3D.compile();
			glBindAttribLocation(shader3D.getUid(), 0, "position");
			glBindAttribLocation(shader3D.getUid(), 1, "normal");
			shader3D.link();
			
			shaderTex2D.loadSource(shader_tex);
			shaderTex2D.compile();
			glBindAttribLocation(shaderTex2D.getUid(), 0, "position");
			glBindAttribLocation(shaderTex2D.getUid(), 1, "textureCoordinates");
			shaderTex2D.link();
			
			std::string src(shader_tex);
            
            boost::algorithm::replace_all(src, "sampler2D", "sampler2DRect");
            boost::algorithm::replace_all(src, "texture2D", "texture2DRect");
			
			shaderTexRect.loadSource(src);
			shaderTexRect.compile();
			glBindAttribLocation(shaderTexRect.getUid(), 0, "position");
			glBindAttribLocation(shaderTexRect.getUid(), 1, "textureCoordinates");
			shaderTexRect.link();
			
			shaderTex2DMask.loadSource(shader_tex_mask);
			shaderTex2DMask.compile();
			glBindAttribLocation(shaderTex2DMask.getUid(), 0, "position");
			glBindAttribLocation(shaderTex2DMask.getUid(), 1, "textureCoordinates");
			shaderTex2DMask.link();
			
			viewport.push(vec4(0,0,0,0));
			projection.push(mat4(1.f));
			modelview.push(mat4(1.f));
			
			texture.push(TextureState());
			blend.push(BlendState());
			shader.push(NULL);
			
            //Need to use GL_MAX_SAMPLES_APPLE for iPhone
            #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
                glGetIntegerv(GL_MAX_SAMPLES_APPLE, &max_fbo_samples);
            #else
                glGetIntegerv(GL_MAX_SAMPLES, &max_fbo_samples);
            #endif
            
			color.set(1.0,1.0,1.0,1.0);
			lineWidth = 1.f;
			pointSize = 1.f;
		}
		
		int max_fbo_samples;
		
		glm::mat4 camera;
		std::stack<glm::vec4> viewport;
		std::stack<glm::mat4> projection;
		std::stack<glm::mat4> modelview;
		
		poColor color;
		float lineWidth;
		float pointSize;
		
		std::stack<TextureState> texture;
		std::stack<BlendState> blend;
		std::stack<poShader*> shader;
		
		poShader shader2D, shader3D, shaderTex2D, shaderTexRect, shaderTex2DMask;
	};
	
	OpenGLState *ogl = NULL;

	void init_graphics() {
		ogl = new OpenGLState;
	}
};

namespace po {
	
	void initGraphics() {
		static boost::once_flag flag;
		boost::call_once(flag, init_graphics);
	}
	
	int maxFBOSamples() {
		return ogl->max_fbo_samples;
	}

	void defaultColorMask() {
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
	
	void defaultStencil() {
		glDisable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 0, 0);
	}
	
	void setColor(float r, float g, float b, float a) {
		setColor(poColor(r,g,b,a));
	}

	void setColor(poColor const& c) {
		ogl->color = c;
	}
	void setColor(poColor const& c, float a) {
		ogl->color = poColor(c,a);
	}
	poColor getColor() { return ogl->color; }
	
	void setLineWidth(float w) {
		ogl->lineWidth = w;
		glLineWidth(w);
	}
	float getLineWidth() { return ogl->lineWidth; }
	
	void setPointSize(float sz) {
		ogl->pointSize = sz;
        
        #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            //This needs to be recorded and set in shader
            //I believe for iOS -Steve
        #else
            glPointSize(sz);
        #endif
	}
	float getPointSize() {return ogl->pointSize; }

	void disableStencil() {
		glDisable(GL_STENCIL_TEST);
	}
	void setupStencilMask(bool c) {
		defaultStencil();
		if(c) glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	}
	void useStencilMask(bool inv) {
		defaultStencil();
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, inv ? 0 : 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void disableBlending() {
		ogl->blend.top() = BlendState();
		ogl->blend.top().apply();
	}
	void enableAlphaBlending() { enableBlendWithFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
	void enableBlendWithFunc(int src, int dst) { enableBlendWithFunc(src, dst, src, dst); }
	void enableBlendWithFunc(int srcRgb, int dstRgb, int srcAlpha, int dstAlpha) {
		BlendState st;
		st.enabled = true;
		st.srcRgbFactor = srcRgb;
		st.dstRgbFactor = dstRgb;
		st.srcAlphaFactor = srcAlpha;
		st.dstAlphaFactor = dstAlpha;
		st.apply();
		ogl->blend.top() = st;
	}
	void saveBlendState() {
		ogl->blend.push(ogl->blend.top());
	}
	void restoreBlendState() {
		ogl->blend.pop();
		ogl->blend.top().apply();
	}

	void disableTexture() {
		TextureState &st = ogl->texture.top();
		glActiveTexture(GL_TEXTURE0 + st.unit);
		glBindTexture(st.target,0);

		st.target = GL_TEXTURE_2D;
		st.bound = 0;
		st.unit = 0;
		st.hasAlpha = false;
	}
	void useTexture(int uid, bool hasAlpha, int target, int unit) {
		TextureState &st = ogl->texture.top();
		st.target = target;
		st.bound = uid;
		st.unit = unit;
		st.hasAlpha = hasAlpha;
		
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, uid);
	}
	void saveTextureState() { ogl->texture.push(ogl->texture.top()); }
	void restoreTextureState() {
		ogl->texture.pop();
		TextureState &st = ogl->texture.top();
		glActiveTexture(GL_TEXTURE0 + st.unit);
		glBindTexture(st.target, st.unit);
	}

	void disableShader() { useShader(NULL); }
	void useShader(poShader* s) {
		ogl->shader.top() = s;
		glUseProgram(s ? s->getUid() : 0);
	}
	void use2DShader() {
		disableTexture();
		useShader(&ogl->shader2D);
	}
	void use3DShader() {
		disableTexture();
		useShader(&ogl->shader3D);
	}
	void useTex2DShader() { useShader(&ogl->shaderTex2D); }
	void useTex2DMaskShader() { useShader(&ogl->shaderTex2DMask); }
	void useTexRectShader() { useShader(&ogl->shaderTexRect); }
	void updateActiveShader() {
		poShader *sh = ogl->shader.top();
		if(sh) {
			// 2D
			sh->uniform("color", ogl->color);
			sh->uniform("mvp", modelviewProjection());
			
			// 3D
			glm::vec4 lightPos(0,1000,0,0);
//			sh->uniform("lightPos", ogl->camera * lightPos);
			sh->uniform("modelView", modelview());
			sh->uniform("projection", projection());
			
			// texture
			TextureState &st = ogl->texture.top();
			sh->uniform("tex", st.unit);
			sh->uniform("isAlphaMask", st.hasAlpha);
		}
	}
	void saveShaderState() { ogl->shader.push(ogl->shader.top()); }
	void restoreShaderState() {
		ogl->shader.pop();
		useShader(ogl->shader.top());
	}
	
	void saveProjection() { ogl->projection.push(ogl->projection.top()); }
	void saveProjectionThenIdentity() { ogl->projection.push(glm::mat4(1.f)); }
	void saveModelview() { ogl->modelview.push(ogl->modelview.top()); }
	void saveModelviewThenIdentity() { ogl->modelview.push(glm::mat4(1.f)); }
	void restoreProjection() { ogl->projection.pop(); }
	void restoreModelview() { ogl->modelview.pop(); }
	void saveViewport() { ogl->viewport.push(ogl->viewport.top()); }
	void restoreViewport() {
		ogl->viewport.pop();
		glm::vec4 vp = ogl->viewport.top();
		glViewport(vp[0],vp[1],vp[2],vp[3]);
	}
	void translate(poPoint off) { ogl->modelview.top() = glm::translate(ogl->modelview.top(), glm::vec3(off.x,off.y,off.z)); }
	void scale(poPoint scl) { ogl->modelview.top() = glm::scale(ogl->modelview.top(), glm::vec3(scl.x,scl.y,scl.z)); }
	void rotate(float angle, poPoint axis) { ogl->modelview.top() = glm::rotate(ogl->modelview.top(), angle, glm::vec3(axis.x,axis.y,axis.z)); }
	void lookAt(poPoint eye, poPoint center, poPoint up) {
		ogl->camera = glm::lookAt(glm::vec3(eye.x,eye.y,eye.z), glm::vec3(center.x,center.y,center.z), glm::vec3(up.x,up.y,up.z));
		ogl->projection.top() *= ogl->camera;
	}
	void setViewport(poRect r) {
		ogl->viewport.top() = glm::vec4(r.x, r.y, r.width, r.height);
		glViewport(r.x, r.y, r.width, r.height);
	}
	void setViewport(float x, float y, float w, float h) {
		ogl->viewport.top() = glm::vec4(x,y,w,h);
		glViewport(x,y,w,h);
	}
	void setCamera(glm::mat4 m) { ogl->camera = m; }
	void setOrthoProjection(float l, float r, float b, float t, float n, float f) {
		ogl->projection.top() = glm::ortho(l, r, b, t, n, f);
	}
	void setPerpsective(float fovy, float aspect, float n, float f) {
		ogl->projection.top() = glm::perspective(fovy, aspect, n, f);
	}
	poRect viewport() {
		glm::vec4 vp = ogl->viewport.top();
		return poRect(vp[0], vp[1], vp[2], vp[3]);
	}
	glm::mat4 modelview() { return ogl->modelview.top(); }
	glm::mat4 projection() { return ogl->projection.top(); }
	glm::mat4 modelviewProjection() { return ogl->projection.top() * ogl->modelview.top(); }
	poPoint globalToLocal(poPoint pt) {
		glm::vec3 r = glm::unProject(glm::vec3(pt.x,pt.y,pt.z), ogl->modelview.top(), ogl->projection.top(), ogl->viewport.top());
		return poPoint(r.x, r.y, r.z);
	}
	poPoint localToGlobal(poPoint pt) {
		glm::vec3 r = glm::project(glm::vec3(pt.x,pt.y,pt.z), ogl->modelview.top(), ogl->projection.top(), ogl->viewport.top());
		return poPoint(r.x, r.y, r.z);
	}

};
