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
//  poShader.h
//  pocode
//
//  Created by Joshua Fisher on 9/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "common.h"

#include <map>
#include <vector>
#include <sstream>

class poShader {
public:
	poShader();
	~poShader();

	void load(const std::string &name);
	void loadSource(std::istream &src);
	bool compile();
	bool link();
	
	GLuint getUid();
	
	std::vector<std::string> getAllAttributes();
	GLint attribLocation(const char *name);
	
	std::vector<std::string> getAllUniforms();
	GLint uniformLocation(const char *name);
	
private:
	GLuint uid;
	GLuint vertexID, fragmentID;
	std::stringstream vertexSource, fragmentSource;
	
	std::map<std::string,GLint> uniformLocations;
	std::map<std::string,GLint> attributeLocations;
};

