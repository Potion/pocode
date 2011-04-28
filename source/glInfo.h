//
//  glInfo.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

static int maxTextureUnits() {
	static int max = 0;
	if(!max) 
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &max);
	return max;
}

