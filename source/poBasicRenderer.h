#pragma once

#include "poColor.h"
#include <glm/glm.hpp>

class poBasicRenderer {
public:
	static poBasicRenderer *get();
	
	void setMVP(const glm::mat4 &mvp);
	void setColor(const poColor &color);
	void setTexture(GLuint tex);
	void disableTexture();
	
	void setFromState();
	
private:
	poBasicRenderer();
	void setFlags();
	
	GLuint uid, flags;
	GLint mvp_uniform, color_uniform, tex_uniform, flags_uniform;
};

