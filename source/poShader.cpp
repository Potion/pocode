//
//  poShader.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poShader.h"

#include "poOpenGLState.h"
#include "poMatrixStack.h"

poShaderProgram::poShader::poShader(GLenum type) {
	this->type = type;
	uid = 0;
}

poShaderProgram::poShader::~poShader() {
	glDeleteShader(uid);
	uid = 0;
}

bool poShaderProgram::poShader::compile() {
	glDeleteShader(uid);
	uid = glCreateShader(type);
	
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
	
	return success == GL_TRUE;
}




poShaderProgram::poShaderProgram(const std::string &vert, const std::string &frag)
:	vertShader(GL_VERTEX_SHADER)
,	fragShader(GL_FRAGMENT_SHADER)
{
	_uid = glCreateProgram();
	_valid = false;
	vertShader.source = vert;
	fragShader.source = frag;
}

//poShaderProgram::poShaderProgram(const std::vector<std::string> &verts, const std::vector<std::string> frags)
//:	vertShader(GL_VERTEX_SHADER)
//,	fragShader(GL_FRAGMENT_SHADER)
//{
//	std::stringstream vert, frag;
//
//	for(int i=0; i<verts.size(); i++)
//		vert << verts[i];
//	for(int i=0; i<frags.size(); i++)
//		frag << frags[i];
//	
//	load(vert.str(), frag.str());
//	link();
//}
//
poShaderProgram::~poShaderProgram() {
	glDeleteProgram(_uid);
	_uid = 0;
}

bool poShaderProgram::compile() {
	bool success = vertShader.compile() && fragShader.compile();
	if(success) {
		glAttachShader(_uid, vertShader.uid);
		glAttachShader(_uid, fragShader.uid);
	}
	return success;
}

bool poShaderProgram::link() {
	if(vertShader.uid && fragShader.uid) {
		glLinkProgram(_uid);
		
		int success = 0;
		glGetProgramiv(_uid, GL_LINK_STATUS, &success);
		if(success != GL_TRUE) {
			int log_size;
			glGetProgramiv(_uid, GL_INFO_LOG_LENGTH, &log_size);
			
			int wrote = 0;
			char *log_contents = new char[log_size];
			glGetProgramInfoLog(_uid, 1024, &wrote, log_contents);
			link_log = std::string(log_contents);
			delete [] log_contents;
		}
		
		_valid = (success == GL_TRUE);
	}
	
	return _valid;
}

bool poShaderProgram::isValid() const { return _valid; }

std::string poShaderProgram::compileLog() const { 
	std::stringstream ss;
	if(!vertShader.uid)
		ss << "vertex log:\n" << vertShader.log << "\n\n";
	if(!fragShader.uid)
		ss << "fragment log:\n" << fragShader.log << "\n\n";
	
	return ss.str();
}

std::string poShaderProgram::linkLog() const {
	return link_log;
}

GLuint poShaderProgram::uid() const {
	return _uid;
}

void poShaderProgram::doSetUp(poObject *obj) {
	poOpenGLState::get()->bindShader(this);
}

void poShaderProgram::doSetDown(poObject *obj) {
	poOpenGLState::get()->unbindShader();
}

void poShaderProgram::setAttribLocation(const std::string &name, GLuint loc) {
	glBindAttribLocation(_uid, loc, name.c_str());
}

void poShaderProgram::uniform1(const std::string &name, int i) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform1i(location, i);
}
void poShaderProgram::uniform1(const std::string &name, float f) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform1f(location, f);
}
void poShaderProgram::uniform1v(const std::string &name, const int *i, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform1iv(location, count, i);
}
void poShaderProgram::uniform1v(const std::string &name, const float *f, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform1fv(location, count, f);
}

void poShaderProgram::uniform2(const std::string &name, int i1, int i2) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform2i(location, i1, i2);
}
void poShaderProgram::uniform2(const std::string &name, float f1, float f2) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform2f(location, f1, f2);
}
void poShaderProgram::uniform2v(const std::string &name, const int *i, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform2iv(location, count, i);
}
void poShaderProgram::uniform2v(const std::string &name, const float *f, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform2fv(location, count, f);
}

void poShaderProgram::uniform3(const std::string &name, int i1, int i2, int i3) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform3i(location, i1, i2, i3);
}
void poShaderProgram::uniform3(const std::string &name, float f1, float f2, float f3) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform3f(location, f1, f2, f3);
}
void poShaderProgram::uniform3(const std::string &name, poPoint point) {
	uniform3(name, point.x, point.y, point.z);
}
void poShaderProgram::uniform3v(const std::string &name, int *i, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform3iv(location, count, i);
}
void poShaderProgram::uniform3v(const std::string &name, const float *f, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform3fv(location, count, f);
}
void poShaderProgram::uniform3v(const std::string &name, const std::vector<poPoint> &points) {
	uniform3v(name, (float*)&points, points.size());
}

void poShaderProgram::uniform4(const std::string &name, int i1, int i2, int i3, int i4) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform4i(location, i1, i2, i3, i4);
}
void poShaderProgram::uniform4(const std::string &name, float f1, float f2, float f3, float f4) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform4f(location, f1, f2, f3, f4);
}
void poShaderProgram::uniform4v(const std::string &name, int *i, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform4iv(location, count, i);
}
void poShaderProgram::uniform4v(const std::string &name, const float *f, int count) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniform4fv(location, count, f);
}

void poShaderProgram::uniformMat4(const std::string &name, const float *f) {
	GLint location = glGetUniformLocation(_uid, name.c_str());
	glUniformMatrix4fv(location, 1, false, f);
}


poShaderProgram *basicProgram1() {
	static poShaderProgram *program = NULL;
	if(!program) {
		std::string vert = 
		"uniform mat4 mvp;\n"
		
		"attribute vec4 position;\n"
		
		"void main() {\n"
		"	gl_Position = mvp * position;\n"
		"}";
		
		std::string frag = 
		"uniform vec4 color;\n"

		"void main() {\n"
		"	gl_FragColor = color;\n"
		"}";
		
		program = new poShaderProgram(vert,frag);
		program->setAttribLocation("position", 0);
		program->compile();
		program->link();
	}
	
	return program;
}

poShaderProgram *basicProgram2() {
	static poShaderProgram *program = NULL;
	if(!program) {
		std::string vert = 
		"uniform mat4 mvp;"
		
		"attribute vec4 position;"
		"attribute vec2 textureCoordinate;"
		
		"varying vec2 texCoord;"
		
		"void main() {"
		"	texCoord = textureCoordinate;"
		"	gl_Position = mvp * position;"
		"}";
		
		std::string frag = 
		"uniform vec4 color;\n"
		"uniform sampler2D tex;\n"

		"varying vec2 texCoord;\n"
		
		"void main() {\n"
		"	vec4 texColor = texture2D(tex,texCoord);\n"
		"	gl_FragColor = color * texColor;\n"
		"}";
		
		program = new poShaderProgram(vert,frag);
		program->setAttribLocation("position", 0);
		program->setAttribLocation("textureCoordinate", 1);
		if(!program->compile())
			std::cout << program->compileLog();
		if(!program->link())
			std::cout << program->linkLog();
	}
	
	return program;
}



