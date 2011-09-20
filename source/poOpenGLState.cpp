//
//  poOpenGLState.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poOpenGLState.h"

#include "poShader.h"
#include "poTexture.h"

static boost::thread_specific_ptr<poOpenGLState> instance;

poOpenGLState::poOpenGLState() {
}

poOpenGLState *poOpenGLState::get() {
	if(!instance.get())
		instance.reset(new poOpenGLState());
	return instance.get();
}

bool poOpenGLState::isEnabled(GLenum attrib) const {
	enumBoolMap_t::const_iterator i = enabled.find(attrib);
	if(i == enabled.end())
		return false;
	return i->second;
}

void poOpenGLState::enable(GLenum attrib) {
	if(!isEnabled(attrib)) {
		glEnable(attrib);
		enabled[attrib] = true;
	}
}

void poOpenGLState::disable(GLenum attrib) {
	if(isEnabled(attrib)) {
		glDisable(attrib);
		enabled[attrib] = false;
	}
}

poTexture *poOpenGLState::boundTexture() const {
	if(textures.empty())
		return NULL;
	return textures.top();
}

void poOpenGLState::bindTexture(poTexture *tex) {
	poTexture *top = textures.empty() ? NULL : textures.top();
	if(!top || top->uid != tex->uid) {
		enable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->uid);
	}
	textures.push(tex);
}

void poOpenGLState::unbindTexture() {
	poTexture *tex=NULL, *top=NULL;
	
	if(!textures.empty()) {
		tex = textures.top();
		textures.pop();
	}
	
	if(!textures.empty())
		top = textures.top();
	
	if(tex && top && tex->uid != top->uid) {
		glBindTexture(GL_TEXTURE_2D, top->uid);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		disable(GL_TEXTURE_2D);
	}
}

poShaderProgram *poOpenGLState::boundShader() const {
	if(shaders.empty())
		return NULL;
	return shaders.top();
}

void poOpenGLState::bindShader(poShaderProgram *prog) {
	poShaderProgram *top = shaders.empty() ? NULL : shaders.top();
	if(!top || top->uid() != prog->uid()) {
		glUseProgram(prog->uid());
	}
	shaders.push(prog);
}

void poOpenGLState::unbindShader() {
	poShaderProgram *prog=NULL, *top=NULL;
	
	if(!shaders.empty()) {
		prog = shaders.top();
		shaders.pop();
	}
	
	if(!shaders.empty())
		top = shaders.top();
	
	if(prog && top && prog->uid() != top->uid()) {
		glUseProgram(top->uid());
	}
	else {
		glUseProgram(0);
	}
}





