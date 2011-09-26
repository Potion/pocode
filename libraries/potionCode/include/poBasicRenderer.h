#pragma once

#include "poColor.h"
#include "poShader.h"

class poBasicRenderer {
public:
	static poBasicRenderer *get();
	void rebuild();
	void setFromState();
	
private:
	poBasicRenderer();
	
	poShader colorShader;
	poShader texturedShader;
	poShader *active;
};

