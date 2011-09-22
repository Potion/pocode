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

class poMatrixStack : boost::noncopyable {
public:
	poMatrixStack();

	void pushModelview();
	// will push this new one on top
	void pushModelview(const glm::mat4 &mat);
	void pushProjection();
	// will push this new one on top
	void pushProjection(const glm::mat4 &mat);
	// will push this new one on top
	void pushViewport(poRect r);

	// restore the previous one
	void popModelview();
	void popProjection();
	void popViewport();

	// get the current top
	glm::mat4 getModelview();
	glm::mat4 getProjection();
	poRect getViewport();
	
	// projection * modelview
	glm::mat4 transformation();

	// set it to opengl
	void load();
	
	// modify the top modelview
	void translate(poPoint t);
	void rotate(float r, poPoint axis);
	void scale(poPoint s);
	
private:
	std::stack<glm::mat4>	projection;
	std::stack<glm::mat4>	modelview;
	std::stack<poRect>		viewport;
};