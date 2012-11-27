/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  Shader.cpp
//  pocode
//
//  Created by Joshua Fisher on 9/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poHelpers.h"
#include "Shader.h"

#include <fstream>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ========================== Shader Building Funcs ==================================
    #pragma mark - Utils -

    //------------------------------------------------------------------------
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
    
    
    //------------------------------------------------------------------------
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
    
    
    //------------------------------------------------------------------------
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
    
    
    //------------------------------------------------------------------------
	std::vector<std::string> allKeys(const std::map<std::string,GLint> &theMap) {
		// http://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap/110228#110228
		using namespace std;
		using namespace boost;
		
		std::vector<std::string> keys;
		transform(theMap.begin(), 
				  theMap.end(), 
				  back_inserter(keys), 
				  boost::bind(&map<string,int>::value_type::first, _1) 
				  );
		return keys;
	}
    
    // -----------------------------------------------------------------------------------
    // ================================= Shader ==========================================
    #pragma mark - Shader -

    Shader::Shader()
    :	uid(0)
    ,	vertexID(0), fragmentID(0)
    {}

    Shader::~Shader() {
        if(uid) {
            glDeleteProgram(uid);
            uid = 0;
        }
    }
    
    
    //------------------------------------------------------------------------
    void Shader::load(const std::string &name) {
        if(uid) {
            glDeleteProgram(uid);
            uid = 0;
        }
        
        loadSource(name);
    }
    
    
    //------------------------------------------------------------------------
    void Shader::loadSource(std::string const& src) {
        using namespace std;
        stringstream uniforms, varyings, vertex, fragment;
        stringstream *target = &uniforms;

        std::istringstream ss(src);
        std::string line;
        while(getline(ss, line)) {
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
        #if defined(OPENGL_ES)
            fragmentSource << "precision mediump float;\n" << uniforms.str() << varyings.str() << fragment.str();
        #else
            fragmentSource << uniforms.str() << varyings.str() << fragment.str();
        #endif
    }
    
    
    //------------------------------------------------------------------------
    bool Shader::compile() {
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
    
    
    //------------------------------------------------------------------------
    bool Shader::link() {
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
    
    
    //------------------------------------------------------------------------
    GLuint Shader::getUid() {
        return uid;
    }
    
    
    //------------------------------------------------------------------------
    std::vector<std::string> Shader::getAllAttributes() {
        return po::allKeys(attributeLocations);
    }
    
    
    //------------------------------------------------------------------------
    GLint Shader::attribLocation(const char *name) {
        if(attributeLocations.find(name) != attributeLocations.end())
            return attributeLocations[name];
        return -1;
    }
    
    
    //------------------------------------------------------------------------
    std::vector<std::string> Shader::getAllUniforms() {
        return po::allKeys(uniformLocations);
    }
    
    
    //------------------------------------------------------------------------
    GLint Shader::uniformLocation(const char *name) {
        if(uniformLocations.find(name) != uniformLocations.end())
            return uniformLocations[name];
        return -1;
    }
    
    
    //------------------------------------------------------------------------
    void Shader::uniform(const char* name, int i) {
        int l = uniformLocation(name);
        if(l >= 0) glUniform1i(l, i);
    }
    
    
    //------------------------------------------------------------------------
    void Shader::uniform(const char* name, float f) {
        int l = uniformLocation(name);
        if(l >= 0) glUniform1f(l, f);
    }
    
    
    //------------------------------------------------------------------------
    void Shader::uniform(const char* name, Point v) {
        int l = uniformLocation(name);
        if(l >= 0) glUniform3fv(l, 1, &v.x);
    }
    
    
    //------------------------------------------------------------------------
    void Shader::uniform(const char* name, Color c) {
        int l = uniformLocation(name);
        if(l >= 0) glUniform4fv(l, 1, &c.R);
    }
    
    
    //------------------------------------------------------------------------
    void Shader::uniform(const char* name, glm::mat4 m) {
        int l = uniformLocation(name);
        if(l >= 0) glUniformMatrix4fv(l, 1, GL_FALSE, &m[0][0]);
    }
}