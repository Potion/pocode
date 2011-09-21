//
//  poOpenGLState.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

class poTexture;
class poShaderProgram;

class poOpenGLState {
public:
	static poOpenGLState *get();
	
	bool isEnabled(GLenum attrib) const;
	void enable(GLenum attrib);
	void disable(GLenum attrib);
	
	poTexture *boundTexture() const;
	void bindTexture(poTexture *tex);
	void unbindTexture();
	
	poShaderProgram *boundShader() const;
	void bindShader(poShaderProgram *prog);
	void unbindShader();
	
private:
	poOpenGLState();

	typedef boost::unordered_map<GLenum,bool> enumBoolMap_t;
	
	enumBoolMap_t enabled;
	
	std::stack<poTexture*> textures;
	std::stack<poShaderProgram*> shaders;
};

