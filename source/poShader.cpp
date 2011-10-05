//
//  poShader.cpp
//  potionCode
//
//  Created by Joshua Fisher on 9/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Helpers.h"
#include "poShader.h"

#include <boost/regex.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace po {
	
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
			
			char *log_contents = new char[log_size];
			glGetShaderInfoLog(uid, 1024, &log_size, log_contents);
			log = std::string(log_contents, log_size);
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
			
			char *log_contents = new char[log_size];
			glGetProgramInfoLog(uid, log_size, &log_size, log_contents);
			log = std::string(log_contents, log_size);
			delete [] log_contents;
			
			glDeleteProgram(uid);
			uid = 0;
			
			return false;
		}
		
		return true;
	}

	void variableNames(const std::string &str, const boost::regex &re, std::map<std::string,GLint> &vars) {
		// http://stackoverflow.com/questions/3122344/boost-c-regex-how-to-get-multiple-matches
		using namespace boost;
		sregex_iterator iter(str.begin(), str.end(), re);
		for( ; iter!=sregex_iterator(); iter++) {
			smatch match = *iter;
			if(match.size() > 1) {
				vars[match[1]] = -1;
			}
		}
	}
	
	std::vector<std::string> allKeys(const std::map<std::string,GLint> &theMap) {
		// http://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap/110228#110228
		using namespace std;
		using namespace boost::lambda;
		
		std::vector<std::string> keys;
		transform(theMap.begin(), 
				  theMap.end(), 
				  back_inserter(keys), 
				  bind(&map<string,int>::value_type::first, boost::lambda::_1) 
				  );
		return keys;
	}
	
}
	
poShader::poShader()
:	uid(0)
,	vertexID(0), fragmentID(0)
{}

poShader::~poShader() {
	if(uid) {
		glDeleteProgram(uid);
		uid = 0;
	}
}

void poShader::load(const std::string &name) {
	if(uid) {
		glDeleteProgram(uid);
		uid = 0;
	}
	
	fs::path current = currentPath();
	lookUpAndSetPath("common");
	
	std::ifstream source((name + ".shader").c_str());
	loadSource(source);
	
	setCurrentPath(current);
}

void poShader::loadSource(std::istream &src) {
	using namespace std;
	stringstream uniforms, varyings, vertex, fragment;
	stringstream *target = &uniforms;
	
	std::string line;
	while(getline(src, line)) {
		boost::algorithm::trim(line);
		
			 if(line == "[[uniforms]]")	target = &uniforms;
		else if(line == "[[varyings]]")	target = &varyings;
		else if(line == "[[vertex]]")	target = &vertex;
		else if(line == "[[fragment]]")	target = &fragment;
		else							*target << line << "\n";
	}
	
	po::variableNames(vertex.str(), boost::regex("attribute .*?\\s+?(.*?);"), attributeLocations);
	po::variableNames(uniforms.str(), boost::regex("uniform .*?\\s+?(.*?);"), uniformLocations);
	
	vertexSource << uniforms.str() << varyings.str() << vertex.str();
#ifdef POTION_IOS
	fragmentSource << "precision mediump float;\n" << uniforms.str() << varyings.str() << fragment.str();
#else
	fragmentSource << uniforms.str() << varyings.str() << fragment.str();
#endif
}

bool poShader::compile() {
	using namespace std;
	
	std::string log;
	vertexID = po::compile(GL_VERTEX_SHADER, vertexSource.str(), log);
	if(vertexID == 0) {
		cout << "vertex:\n" << log;
		return false;
	}
	
	log = "";
	fragmentID = po::compile(GL_FRAGMENT_SHADER, fragmentSource.str(), log);
	if(fragmentID == 0) {
		cout << "fragment:\n" << log;
		glDeleteShader(vertexID);
		return false;
	}
	
	uid = glCreateProgram();
	glAttachShader(uid, vertexID);
	glAttachShader(uid, fragmentID);
	
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return true;
}
	
bool poShader::link() {
	std::string log;
	if(!po::link(uid, log)) {
		std::cout << "link:\n" << log;
		glDeleteProgram(uid);
		uid = 0;
		return false;
	}
	
	std::map<std::string,GLint>::iterator iter = uniformLocations.begin();
	for( ; iter!=uniformLocations.end(); iter++) {
		uniformLocations[iter->first] = glGetUniformLocation(uid, iter->first.c_str());
//		std::cout << iter->first << ": " << uniformLocations[iter->first] << "\n";
	}
	
	return true;
}

GLuint poShader::getUid() {
	return uid;
}

std::vector<std::string> poShader::allAttributes() {
	return po::allKeys(attributeLocations);
}

GLint poShader::attribLocation(const char *name) {
	if(attributeLocations.find(name) != attributeLocations.end())
		return attributeLocations[name];
	return -1;
}

std::vector<std::string> poShader::allUniforms() {
	return po::allKeys(uniformLocations);
}

GLint poShader::uniformLocation(const char *name) {
	if(uniformLocations.find(name) != uniformLocations.end())
		return uniformLocations[name];
	return -1;
}

