#pragma once

#include "poColor.h"
#include <glm/glm.hpp>

class poBasicRenderer {
public:
	static poBasicRenderer *get();
	
	void setMVP(const glm::mat4 &mvp);
	void setColor(const poColor &color);
	void setTexture(GLuint tex);
	void setFlags(int flags);
	
	void setFromState();
	
private:
	poBasicRenderer();
	int determineFlagsFromState();
	
	GLuint uid, flags;
	GLint mvp_uniform, color_uniform, tex_uniform;
	GLint flags_uniform, time_uniform;
};

