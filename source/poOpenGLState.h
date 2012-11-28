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
//  poOpenGLState.h
//  pocode
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poColor.h"
#include "poPoint.h"
#include "poRect.h"
#include <glm/glm.hpp>

namespace po {
    class Object;
    class Shader;
    
	void initGraphics();

	int maxFBOSamples();
	
	void defaultColorMask();
	void defaultStencil();
	void defaultBlending();

	void setColor(Color const& c);
	void setColor(Color const& c, float alpha);
	Color getColor();
	void setLineWidth(float w);
	float getLineWidth();
	void setPointSize(float sz);
	float getPointSize();
	
	void disableStencil();
	void setupStencilMask(bool clear=true);
	void useStencilMask(bool inverse=false);
	
	void disableBlending();
	void enableAlphaBlending();
	void enableBlendWithFunc(int src, int dst);
	void enableBlendWithFunc(int srcRgb, int dstRgb, int srcAlpha, int dstAlpha);
	void saveBlendState();
	void restoreBlendState();
	
	void disableTexture();
	void useTexture(int uid, bool hasAlpha, int target=GL_TEXTURE_2D, int unit=0);
	void saveTextureState();
	void restoreTextureState();
	
	void disableShader();
	void useShader(Shader* s);
	void use2DShader();
	void use3DShader();
	void useTex2DShader();
	void useTexRectShader();
	void useTex2DMaskShader();
	void updateActiveShader();
	void saveShaderState();
	void restoreShaderState();

	void enableVertexAttrib(int attr, bool b);
	
	void saveProjection();
	void saveProjectionThenIdentity();
	void saveModelview();
	void saveModelviewThenIdentity();
	void restoreProjection();
	void restoreModelview();
	void saveViewport();
	void restoreViewport();
	void translate(Point off);
	void scale(Point scl);
	void rotate(float angle, Point axis);
	void lookAt(Point eye, Point center, Point up);
	void setViewport(Rect r);
	void setViewport(float l, float r, float t, float b);
	void setCamera(glm::mat4 m);
	void setOrthoProjection(float l, float r, float b, float t, float n=-1, float f=1);
	void setPerpsective(float fovy, float aspect, float n, float f);
	Rect viewport();
	glm::mat4 projection();
	glm::mat4 modelview();
	glm::mat4 modelviewProjection();
	Point globalToLocal(Point pt);
	Point localToGlobal(Point pt);
};


