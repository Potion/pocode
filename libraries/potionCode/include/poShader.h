//
//  poShader.h
//  potionCode
//
//  Created by Joshua Fisher on 9/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

class poShader {
public:
	poShader();
	~poShader();

	void load(const std::string &name);
	void loadSource(std::istream &src);
	bool compile();
	bool link();
	
	GLuint getUid();
	
	std::vector<std::string> allAttributes();
	GLint attribLocation(const char *name);
	
	std::vector<std::string> allUniforms();
	GLint uniformLocation(const char *name);
	
private:
	GLuint uid;
	GLuint vertexID, fragmentID;
	std::stringstream vertexSource, fragmentSource;
	
	std::map<std::string,GLint> uniformLocations;
	std::map<std::string,GLint> attributeLocations;
};

