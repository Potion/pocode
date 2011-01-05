#ifndef _PO_SHADER_H_
#define _PO_SHADER_H_

#include "poCommon.h"
#include "poObjectModifier.h"

void getGLVersion(int* major, int* minor);
void getGLSLVersion(int* major, int* minor);
char* readShader(const char* filename);

class poShader : public poObjectModifier
{
public:
	poShader();
	poShader(const char* vert, const char* frag);
	virtual ~poShader();

	void setUniform1i(const char* name, int val);
	void setUniform1f(const char* name, float val);
	void setUniform2f(const char* name, float val1, float val2);
	void setUniform3f(const char* name, float val1, float val2, float val3);
	void setUniform4f(const char* name, float val1, float val2, float val3, float val4);
	
	void printShaderLog(GLuint shaderID);
	void printProgramLog();

	virtual void setup(poObject*);
	virtual void setdown(poObject*);

private:
	void construct();

	void setVertexSource(const char** source, GLsizei count);
	void setFragmentSource( const char** source, GLsizei count);
	bool compile();
	bool compileShader(GLuint shaderID);
	bool linkShaders();

	GLuint vertShaderID;
	GLuint fragShaderID;
	GLuint programID;
	
	bool didCompile;
	bool activeVertShader;
	bool activeFragShader;
};

#endif