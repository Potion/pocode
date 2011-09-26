#include "poBasicRenderer.h"
#include "poOpenGLState.h"
#include "Helpers.h"

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
			glUniform1i(texturedShader.uniformLocation("tex"), 0);
			active = &texturedShader;
		}
	}
	else if(active->getUid() != colorShader.getUid()) {
		glUseProgram(colorShader.getUid());
		active = &colorShader;
	}
	
	glUniformMatrix4fv(active->uniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.transformation()));
	glUniform4fv(active->uniformLocation("color"), 1, &ogl->color.R);
}

void poBasicRenderer::rebuild() {
	glUseProgram(0);
	
	colorShader.load("colored");
	colorShader.compile();
	glBindAttribLocation(colorShader.getUid(), 0, "position");
	colorShader.link();
	
	texturedShader.load("textured");
	texturedShader.compile();
	glBindAttribLocation(texturedShader.getUid(), 0, "position");
	glBindAttribLocation(texturedShader.getUid(), 1, "textureCoordinates");
	texturedShader.link();
	
	active = &colorShader;
	glUseProgram(active->getUid());
}


