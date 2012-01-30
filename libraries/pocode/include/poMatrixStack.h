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
//  poMatrixStack.h
//  glmTest
//
//  Created by Joshua Fisher on 8/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#define GLM_PRECISION_MEDIUMP_INT 
#define GLM_PRECISION_MEDIUMP_FLOAT

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stack>

#include "poRect.h"

class poMatrixStack {
public:
	poMatrixStack();

	void pushModelview();
	// will push this new one on top
	void pushModelview(const glm::mat4 &mat);
	void pushProjection();
	// will push this new one on top
	void pushProjection(const glm::mat4 &mat);
	// will push this new one on top
	void pushViewport();
	void pushViewport(poRect r);
    
	// restore the previous one
	void popModelview();
	void popProjection();
	void popViewport();

	// get the current top
	glm::mat4 getModelview();
	glm::mat4 getProjection();
	poRect getViewport();
	
    void setCameraMatrix(const glm::mat4 &mat) { camera = mat; }
    glm::mat4 getCameraMatrix() { return camera; }

	// projection * modelview
	glm::mat4 transformation();
	
	// modify the top modelview
	void translate(poPoint t);
	void rotate(float r, poPoint axis);
	void scale(poPoint s);
	
private:
	std::stack<glm::mat4>	projection;
	std::stack<glm::mat4>	modelview;
	std::stack<poRect>		viewport;
    glm::mat4               camera;
};


