#include "poBasicRenderer.h"
#include "poOpenGLState.h"
#include "Helpers.h"

const char * colored_shader = 
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

const char * textured_shader = 
"	[[uniforms]]							\n"
"	uniform mat4 mvp;						\n"
"	uniform vec4 color;						\n"
"	uniform sampler2D tex;					\n"
"	uniform int isAlphaMask;				\n"

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
		
"		if(bool(isAlphaMask)) {				\n"
"			gl_FragColor = vec4(1.0,1.0,1.0,texColor.a) * color;\n"
"		}									\n"
"	}										\n";

static boost::thread_specific_ptr<poBasicRenderer> instance;

poBasicRenderer *poBasicRenderer::get() {
	if(!instance.get())
		instance.reset(new poBasicRenderer());
	return instance.get();
}

poBasicRenderer::poBasicRenderer()
{
	rebuild();
}

void poBasicRenderer::setFromState() {
	poOpenGLState *ogl = poOpenGLState::get();
	
	// but texturing isn't on by default
	if(ogl->texture.bound_id > 0) {
		if(active->getUid() != texturedShader.getUid()) {
			glUseProgram(texturedShader.getUid());
			active = &texturedShader;
		}
		glUniform1i(texturedShader.uniformLocation("tex"), 0);
		glUniform1i(texturedShader.uniformLocation("isAlphaMask"), ogl->texture.is_mask);
	}
	else if(active->getUid() != colorShader.getUid()) {
		glUseProgram(colorShader.getUid());
		active = &colorShader;
	}
	
	glUniformMatrix4fv(active->uniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.transformation()));
	glUniform4fv(active->uniformLocation("color"), 1, &ogl->color.R);
	
	poRect viewport = ogl->matrix.getViewport();
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void poBasicRenderer::rebuild() {
	glUseProgram(0);
	
	std::istringstream color(colored_shader);
	colorShader.loadSource(color);
	if(!colorShader.compile())
		throw std::runtime_error("color shader didn't compile");
	glBindAttribLocation(colorShader.getUid(), 0, "position");
	if(!colorShader.link())
		throw std::runtime_error("color shader didn't link");
	
	std::istringstream texture(textured_shader);
	texturedShader.loadSource(texture);
	if(!texturedShader.compile())
		throw std::runtime_error("texture shader didn't compile");
	glBindAttribLocation(texturedShader.getUid(), 0, "position");
	glBindAttribLocation(texturedShader.getUid(), 1, "textureCoordinates");
	if(!texturedShader.link())
		throw std::runtime_error("texture shader didn't link");
	
	active = &colorShader;
	glUseProgram(active->getUid());
}

