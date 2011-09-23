#include "poBasicRenderer.h"
#include "poOpenGLState.h"
#include "Helpers.h"

const char* default_vert = 
	"uniform mat4 mvp;\n"

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

static boost::thread_specific_ptr<poBasicRenderer> instance;

poBasicRenderer *poBasicRenderer::get() {
	if(!instance.get())
		instance.reset(new poBasicRenderer());
	return instance.get();
}

poBasicRenderer::poBasicRenderer()
:	uid(0)
,	mvp_uniform(-1)
,	color_uniform(-1)
,	tex_uniform(-1)
,	flags_uniform(-1)
{
	std::string log;
	
	// the vertex shader
	GLuint vert = compile(GL_VERTEX_SHADER, default_vert, log);
	if(!vert) {
		std::cout << "vertex:\n" << log;
		return;
	}
	
	GLuint frag = compile(GL_FRAGMENT_SHADER, default_frag, log);
	if(!frag) {
		std::cout << "fragment:\n" << log;
		return;
	}
	
	uid = glCreateProgram();
	glAttachShader(uid, vert);
	glAttachShader(uid, frag);
	
	glBindAttribLocation(uid, 0, "position");
	glBindAttribLocation(uid, 1, "textureCoordinates");
	
	if(!link(uid, log)) {
		glDeleteShader(vert);
		glDeleteShader(frag);
		
		std::cout << "link:\n" << log;
		glDeleteProgram(uid);
		uid = 0;
		return;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	mvp_uniform = glGetUniformLocation(uid, "mvp");
	color_uniform = glGetUniformLocation(uid, "color");
	tex_uniform = glGetUniformLocation(uid, "tex");
	flags_uniform = glGetUniformLocation(uid, "flags");
	
	glUseProgram(uid);
}

void poBasicRenderer::setMVP(const glm::mat4 &mvp) {
	glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp));
}

void poBasicRenderer::setColor(const poColor &color) {
	glUniform4fv(color_uniform, 1, &color.R);
}

void poBasicRenderer::setTexture(GLuint uid) {
	glUniform1i(tex_uniform, 0);
}

void poBasicRenderer::setFlags(int flags) {
	glUniform1i(flags_uniform, flags);
}

void poBasicRenderer::setFromState() {
	poOpenGLState *ogl = poOpenGLState::get();
	
	setMVP(ogl->matrix.transformation());
	setColor(ogl->color);
	setTexture(ogl->texture.bound_id);
	setFlags(determineFlagsFromState());
}

int poBasicRenderer::determineFlagsFromState() {
	poOpenGLState *ogl = poOpenGLState::get();
	
	flags = 0;
	if(ogl->texture.bound_id)
		flags = 1;
	if(ogl->texture.is_mask)
		flags |= 2;
	
	return flags;
}

