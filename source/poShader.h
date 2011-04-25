//
//  poShader.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

class poShader {
public:
	poShader(const std::string &name, GLenum type);
	poShader(const std::string &name, GLenum type, const std::string &source);
	poShader(const std::string &name, GLenum type, const std::vector<std::string> &source);
	
	bool compile();
	std::string log();
	
private:
	uint32_t uid;
};

class poShaderProgram {
	poShaderProgram(const std::string &name);
	poShaderProgram(const sdt::string &name, const std::vector<poShader> &source);
	
	bool complie();
	std::string log();
	
	void setup();
	void setdown();
	void uniform(const std::string &name, float f);
	void uniform(const std::string &name, float *f, int count);
	void uniform(const std::string &name, int i);
	void uniform(const std::string &name, int *i, int count);
	void uniform(const std::string &name, 
};