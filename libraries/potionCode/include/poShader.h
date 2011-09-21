#pragma once

#include "poPoint.h"
#include "poObjectModifier.h"

class poShaderProgram : public poObjectModifier 
{
public:
	poShaderProgram(const std::string &vert, const std::string &frag);
	~poShaderProgram();

	void setAttribLocation(const std::string &name, GLuint loc);
	
	bool compile();
	bool link();
	
	bool isValid() const;
	std::string compileLog() const;
	std::string linkLog() const;
	GLuint uid() const;

	void uniform1(const std::string &name, int i);
	void uniform1(const std::string &name, float f);
	void uniform1v(const std::string &name, const int *i, int count);
	void uniform1v(const std::string &name, const float *f, int count);

	void uniform2(const std::string &name, int i1, int i2);
	void uniform2(const std::string &name, float f1, float f2);
	void uniform2v(const std::string &name, const int *i, int count);
	void uniform2v(const std::string &name, const float *f, int count);
	
	void uniform3(const std::string &name, int i1, int i2, int i3);
	void uniform3(const std::string &name, float f1, float f2, float f3);
	void uniform3(const std::string &name, poPoint point);
	void uniform3v(const std::string &name, int *i, int count);
	void uniform3v(const std::string &name, const float *f, int count);
	void uniform3v(const std::string &name, const std::vector<poPoint> &points);

	void uniform4(const std::string &name, int i1, int i2, int i3, int i4);
	void uniform4(const std::string &name, float f1, float f2, float f3, float f4);
	void uniform4v(const std::string &name, int *i, int count);
	void uniform4v(const std::string &name, const float *f, int count);
	
	void uniformMat4(const std::string &name, const float *f);
	
protected:
	virtual void doSetUp(poObject *obj);
	virtual void doSetDown(poObject *obj);

private:
	class poShader {
	public:
		poShader(GLenum type);
		~poShader();
		bool compile();
		
		std::string source;
		std::string log;
		uint32_t uid;
		GLenum type;
	};
	
	GLuint _uid;
	bool _valid;
	std::string link_log;
	poShader vertShader, fragShader;
};

poShaderProgram *basicProgram1();
poShaderProgram *basicProgram2();

