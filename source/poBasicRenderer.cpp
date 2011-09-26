#include "poBasicRenderer.h"
#include "poOpenGLState.h"
#include "Helpers.h"

<<<<<<< HEAD
=======
const char* default_vert = 
	"uniform mat4 mvp;\n"

    "//precision mediump float;\n"

    "attribute vec4 position;\n"
	"attribute vec2 textureCoordinates;\n"

	"varying vec2 texCoord;\n"

	"void main() {\n"
	"	texCoord = textureCoordinates;\n"
	"	gl_Position = mvp * position;\n"
	"}";

const char* default_frag = 
	"const int TEXTURE = 1;\n"
	"const int TEXTURE_IS_MASK = 2;\n"

    "//precision mediump float;"

	"uniform sampler2D tex;\n"
	"uniform vec4 color;\n"
	"uniform int flags;\n"

	"varying vec2 texCoord;\n"

	"void main() {\n"
	"	if(bool(flags & TEXTURE)) {\n"
	"		vec4 texColor = texture2D(tex,texCoord);\n"
	"		if(bool(flags & TEXTURE_IS_MASK))\n"
	"			gl_FragColor = vec4(1.0,1.0,1.0,texColor.a) * color;\n"
	"		else\n"
	"			gl_FragColor = texture2D(tex,texCoord) * color;\n"
	"	}\n"
	"	else {\n"
	"		gl_FragColor = color;\n"
	"	}\n"
	"}";

GLuint compile(GLenum type, const std::string &source, std::string &log) {
	log = "";
	GLuint uid = glCreateShader(type);
	
	const char *src = source.c_str();
	glShaderSource(uid, 1, &src, NULL);
	
	glCompileShader(uid);
	
	int success;
	glGetShaderiv(uid, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		int log_size;
		glGetShaderiv(uid, GL_INFO_LOG_LENGTH, &log_size);
		
		int wrote = 0;
		char *log_contents = new char[log_size];
		glGetShaderInfoLog(uid, 1024, &wrote, log_contents);
		log = std::string(log_contents, wrote);
		delete [] log_contents;
		
		glDeleteShader(uid);
		uid = 0;
	}
	
	return uid;
}

bool link(GLuint uid, std::string &log) {
	log = "";
	glLinkProgram(uid);
	
	int success = 0;
	glGetProgramiv(uid, GL_LINK_STATUS, &success);
	if(success != GL_TRUE) {
		int log_size;
		glGetProgramiv(uid, GL_INFO_LOG_LENGTH, &log_size);
		
		int wrote = 0;
		char *log_contents = new char[log_size];
		glGetProgramInfoLog(uid, 1024, &wrote, log_contents);
		log = std::string(log_contents);
		delete [] log_contents;
	}
}

>>>>>>> c15b84ae664d0f725d177ed1894358dfa7484043
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


