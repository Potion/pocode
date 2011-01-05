#include "poShader.h"

void getGLVersion(int* major, int* minor) {
	const char* versionString = (const char*)glGetString(GL_VERSION);
	if(versionString == 0 || sscanf(versionString, "%d.%d", major, minor) != 2) {
		*major = *minor = 0;
		printf("ERROR: getGLVersion: invalid GL_VERSION format\n");
	}
}

void getGLSLVersion(int* major, int* minor) {
	int glMajor, glMinor;
	getGLVersion(&glMajor, &glMinor);
	
	*major = *minor = 0;
	if(glMajor == 1) {
		const char* extensionString = (const char*)glGetString(GL_EXTENSIONS);
		if(extensionString != 0 && strstr(extensionString, "GL_ARB_shading_language_100") != NULL) {
			*major = 1;
			*minor = 0;
		}
	} else if(glMajor >= 2) {
		const char* versionString = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
		if((versionString == 0) || (sscanf(versionString, "%d.%d", major, minor) != 2)) {
			*major = *minor = 0;
			printf("ERROR: getGLSLVersion: invalid GL_SHADING_LANGUAGE_VERSION format\n");
		}
	}
}

char* readShader(const char* filename) {
	FILE* file = fopen(filename, "r");
	
	if(file == NULL) {
		printf("ERROR: readShader: couldn't open %s\n", filename);
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* source = new char[size];
	fread(source, size, 1, file);
	fclose(file);
	
	return source;
}

poShader::poShader() 
:	didCompile(false), activeVertShader(false), activeFragShader(false)
{
	construct();
}

poShader::poShader(const GLchar* vert, const GLchar* frag) 
:	didCompile(false), activeVertShader(false), activeFragShader(false)
{
	const char** vertsource = new const char*[1];
	const char** fragsource = new const char*[1];
	vertsource[0] = readShader(vert);
	fragsource[0] = readShader(frag);

	construct();
	setVertexSource(vertsource, 1);
	setFragmentSource(fragsource, 1);
	compile();
	
	delete vertsource[0];
	delete fragsource[0];
	delete [] vertsource;
	delete [] fragsource;
	
	glUseProgram(0);
}

poShader::~poShader() 
{
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);
	glDeleteProgram(programID);
}

void poShader::construct()
{
	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
}

void poShader::setVertexSource(const GLchar** source, GLsizei count) 
{
	glShaderSource(vertShaderID, count, source, NULL);
	activeVertShader = true;
}

void poShader::setFragmentSource(const GLchar** source, GLsizei count) 
{
	glShaderSource(fragShaderID, count, source, NULL);
	activeFragShader = true;
}

void poShader::setUniform1i(const char* name, int val) 
{
	glUseProgram(programID);
	GLint uniformLoc = glGetUniformLocation(programID, name);
	if(uniformLoc == -1) {
		printf("ERROR: setUniform: can't find location for uniform variable %s\n", name);
		return;
	}
	glUniform1i(uniformLoc, val);
	glUseProgram(0);
}

void poShader::setUniform1f(const char* name, float val) 
{
	glUseProgram(programID);
	GLint uniformLoc = glGetUniformLocation(programID, name);
	if(uniformLoc == -1) {
		printf("ERROR: setUniform: can't find location for uniform variable %s\n", name);
		return;
	}
	glUniform1f(uniformLoc, val);
	glUseProgram(0);
}

void poShader::setUniform2f(const char* name, float val1, float val2) 
{
	glUseProgram(programID);
	GLint uniformLoc = glGetUniformLocation(programID, name);
	if(uniformLoc == -1) {
		printf("ERROR: setUniform: can't find location for uniform variable %s\n", name);
		return;
	}
	glUniform2f(uniformLoc, val1, val2);
	glUseProgram(0);
}

void poShader::setUniform3f(const char* name, float val1, float val2, float val3) 
{
	glUseProgram(programID);
	GLint uniformLoc = glGetUniformLocation(programID, name);
	if(uniformLoc == -1) {
		printf("ERROR: setUniform: can't find location for uniform variable %s\n", name);
		return;
	}
	glUniform3f(uniformLoc, val1, val2, val3);
	glUseProgram(0);
}

void poShader::setUniform4f(const char* name, float val1, float val2, float val3, float val4) 
{
	glUseProgram(programID);
	GLint uniformLoc = glGetUniformLocation(programID, name);
	if(uniformLoc == -1) {
		printf("ERROR: setUniform: can't find location for uniform variable %s\n", name);
		return;
	}
	glUniform4f(uniformLoc, val1, val2, val3, val4);
	glUseProgram(0);
}

bool poShader::compile() 
{
	didCompile = true;
	if(activeVertShader)
		didCompile &= compileShader(vertShaderID);
	if(activeFragShader)
		didCompile &= compileShader(fragShaderID);

	didCompile &= linkShaders();
	return didCompile;
}

bool poShader::compileShader(GLuint shaderID) 
{
	GLint compileStatus;
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE) {
		printShaderLog(shaderID);
	} else {
		glAttachShader(programID, shaderID);
	}
	return compileStatus == GL_TRUE;
}

bool poShader::linkShaders() 
{
	GLint linkStatus;
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if(linkStatus != GL_TRUE) {
		printProgramLog();
	}
	return linkStatus == GL_TRUE;
}

void poShader::printShaderLog(GLuint shaderID) 
{
	GLint size;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &size);
	if(size > 0) {
		GLchar* log = new GLchar[size];
		glGetShaderInfoLog(shaderID, size, NULL, log);
		printf("%s", log);
		delete [] log;
	}
}

void poShader::printProgramLog() 
{
	GLint size;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &size);
	if(size > 0)
	{
		GLchar* progLog = new GLchar[size];
		glGetProgramInfoLog(programID, size, NULL, progLog);
		printf("%s", progLog);
		delete [] progLog;
	}
}

void poShader::setup(poObject*)
{
	glUseProgram(programID);
}

void poShader::setdown(poObject*) 
{
	glUseProgram(0);
}
