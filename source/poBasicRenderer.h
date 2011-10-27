#pragma once

#include "poColor.h"
#include "poShader.h"

class poBasicRenderer {
public:
	static poBasicRenderer *get();
	void rebuild();
	void setFromState();
    
    void setFor3D();
	
private:
	poBasicRenderer();
	
	poShader colorShader;
	poShader texturedShader;
    poShader geom3DShader;
	poShader *active;
};

