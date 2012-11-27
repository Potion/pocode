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
class poObject;
class poShader;

namespace po {
	void initGraphics();

	int maxFBOSamples();
	
	void defaultColorMask();
	void defaultStencil();
	void defaultBlending();

	void setColor(poColor const& c);
	void setColor(poColor const& c, float alpha);
	poColor getColor();
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
	void useShader(poShader* s);
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
	void translate(poPoint off);
	void scale(poPoint scl);
	void rotate(float angle, poPoint axis);
	void lookAt(poPoint eye, poPoint center, poPoint up);
	void setViewport(poRect r);
	void setViewport(float l, float r, float t, float b);
	void setCamera(glm::mat4 m);
	void setOrthoProjection(float l, float r, float b, float t, float n=-1, float f=1);
	void setPerpsective(float fovy, float aspect, float n, float f);
	poRect viewport();
	glm::mat4 projection();
	glm::mat4 modelview();
	glm::mat4 modelviewProjection();
	poPoint globalToLocal(poPoint pt);
	poPoint localToGlobal(poPoint pt);
};


