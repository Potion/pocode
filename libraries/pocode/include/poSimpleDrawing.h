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

// NOTES
//
// poSimpleDrawing contains all the basic drawing methods used in pocode
//
// These methods can used to draw shapes and lines and they are
// commonly called in the draw() function
//
// All the drawing methods are grouped under the namespace po.
// You can draw a stroked rectangle as follows:
//		po::drawStrokedRect(0,0,100,100);
// 


namespace po {
    
	// Set the color of the shapes or strokes that are drawn after this method call
	void setColor(float R, float G, float B, float A);
    void setColor(poColor color);
	void setColor(poColor color, float alpha);
	
	// Set the stroke width of the strokes that are drawn after this method call ( must be at least 1 )
	void setStrokeWidth(int strokeWidth);
	
	// Draw a line
	void drawLine(poPoint a, poPoint b);
	
	// Draw a string
	void drawString(const std::string &str, poFont *font, poPoint position, int ptSize=0, float tracking=1.f);
    
	// Draw a stroked rectangle
	void drawStrokedRect(float x, float y, float w, float h);
	void drawStrokedRect(poRect rect);
	
	// Draw a filled rectangle
	void drawFilledRect(float x, float y, float w, float h);
	void drawFilledRect(poRect rect);
	
	// Draw a rectangle with a texture placed on it
	// By default, the texture will be placed within the rectangle at actual size. This means that
    // the texture image may extend beyond the bounds of the shape or may not be large enough
    // to fill the shape. Use poTextureFitOption and poAlignment to determine how a texture is placed
    // into the shape. These options are listed in poEnums.h.
	void drawTexturedRect(poTexture *tex);
	void drawTexturedRect(poTexture *tex, float x, float y, float w, float h);
	void drawTexturedRect(poTexture *tex, poRect rect);
	void drawTexturedRect(poTexture *tex, poRect rect, poRect coords);
	void drawTexturedRect(poTexture *tex, poRect rect, poTextureFitOption fit, poAlignment align);
	
	// Draw a stroked polygon
	void drawStrokedPolygon(const std::vector<poPoint> &points);
	void drawStrokedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	
	// Draw a filled polygon
	void drawFilledPolygon(const std::vector<poPoint> &points);
	void drawFilledPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	
	// Draw a polygon with a texture placed on it
	// Using this method gives you the option of specifying texture indeces and coordinates
	// so you have more control over how the texture is placed into the shape
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, poTextureFitOption fit, poAlignment align);
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords);
	void drawTexturedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords);
	
	// drawPoints allows you to draw a shape and specify the GLenum type
	void drawPoints(const std::vector<poPoint> &points, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type);
	
	// The "generateStroke" method creates a very high quality stroke.
    // This stroke is actually drawn as a very thin filled shape. 
	// Given a list of points that define the corners of a certain shape, 
	// the method returns another list of points that can be used to draw the
	// stroke as a thin shape around the original shape.
	// For example you can draw a triangle as follows:
	//		std::vector<poPoint> shapePoints;
	//		shapePoints.push_back(poPoint(100,100));
	//		shapePoints.push_back(poPoint(300,100));
	//		shapePoints.push_back(poPoint(300,300));
	//		
	//		std::vector<poPoint> strokePoints = po::generateStroke(shapePoints, 10, true);
	//		po::drawPoints(strokePoints, GL_TRIANGLE_STRIP);
	
	std::vector<poPoint> generateStroke(std::vector<poPoint> &points, 
										int strokeWidth, 
										bool close = false,
										poStrokePlacementProperty place = PO_STROKE_PLACE_CENTER, 
										poStrokeJoinProperty join = PO_STROKE_JOIN_MITRE, 
										poStrokeCapProperty cap = PO_STROKE_CAP_BUTT);
	
	// The "generateOval" method returns a list of points that can be used
	// to draw a 2D oval shape, given an horizontal and vertical radius.
	// The third argument specifies the number of points to be used.
	// For a smooth oval or circle, this should generally be about 1/3 the width or height of the shape.
	// For example, a 100x100 circle, should use approximately 33 points.
	// An oval can be drawn as follows:
	//		std::vector<poPoint> ovalPoints = po::generateOval(100, 100);
	//		po::drawPoints(ovalPoints, GL_TRIANGLE_FAN);
	
	std::vector<poPoint> generateOval(float xRad, float yRad, uint resolution=50);
}


