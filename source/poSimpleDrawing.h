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

#pragma once


#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poTexture.h"
#include "poObject.h"
#include "poFont.h"

namespace po {
	
	std::vector<poPoint> generateStroke(std::vector<poPoint> &points, 
										int strokeWidth, 
										bool close = false,
										poStrokePlacementProperty place = PO_STROKE_PLACE_CENTER, 
										poStrokeJoinProperty join = PO_STROKE_JOIN_MITRE, 
										poStrokeCapProperty cap = PO_STROKE_CAP_BUTT);
	std::vector<poPoint> generateOval(float xRad, float yRad, uint resolution=50);
    
    void setColor(poColor color);
	void setColor(poColor color, float new_alpha);
	
	void drawLine(poPoint a, poPoint b);
    
	void drawStrokedRect(float x, float y, float w, float h);
	void drawStrokedRect(poRect rect);
	void drawFilledRect(float x, float y, float w, float h);
	void drawFilledRect(poRect rect);
	
	void drawTexturedRect(poTexture *tex);
	void drawTexturedRect(poTexture *tex, float x, float y, float w, float h);
	void drawTexturedRect(poTexture *tex, poRect rect);
	void drawTexturedRect(poTexture *tex, poRect rect, poRect coords);
	void drawTexturedRect(poTexture *tex, poRect rect, poTextureFitOption fit, poAlignment align);
	
	void drawStrokedPolygon(const std::vector<poPoint> &points);
	void drawStrokedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	void drawFilledPolygon(const std::vector<poPoint> &points);
	void drawFilledPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, poTextureFitOption fit, poAlignment align);
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords);
	void drawTexturedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords);
	
	void drawPoints(const std::vector<poPoint> &points, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type);
    
	void drawString(const std::string &str, poFont *font, poPoint position, int ptSize=0, float tracking=1.f);

}


