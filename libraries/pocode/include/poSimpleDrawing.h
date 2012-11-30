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

#include "poOpenGLState.h"

namespace po {
    
	// Draw a line
	void drawLine(Point a, Point b);
	
	// Draw a string
	void drawString(const std::string &str, Font *font, Point position, int ptSize=0, float tracking=1.f);
    
	// Draw a stroked rectangle
	void drawStrokedRect(float x, float y, float w, float h);
	void drawStrokedRect(Rect rect);
	
	// Draw a filled rectangle
	void drawFilledRect(float x, float y, float w, float h);
	void drawFilledRect(Rect rect);
	
	void drawFilledCircle(float x, float y, float rad);
	
	// Draw a rectangle with a texture placed on it
	// By default, the texture will be placed within the rectangle at actual size. This means that
    // the texture image may extend beyond the bounds of the shape or may not be large enough
    // to fill the shape. Use TextureFitOption and Alignment to determine how a texture is placed
    // into the shape. These options are listed in poEnums.h.
	void drawTexturedRect(Texture *tex);
	void drawTexturedRect(Texture *tex, float x, float y, float w, float h);
	void drawTexturedRect(Texture *tex, Rect rect);
	void drawTexturedRect(Texture *tex, Rect rect, Rect coords);
	void drawTexturedRect(Texture *tex, Rect rect, TextureFitOption fit, Alignment align);
	
	// Draw a stroked polygon
	void drawStrokedPolygon(const std::vector<Point> &points);
	void drawStrokedPolygon(const std::vector<Point> &points, const std::vector<unsigned short> &indices);
	
	// Draw a filled polygon
	void drawFilledPolygon(const std::vector<Point> &points);
	void drawFilledPolygon(const std::vector<Point> &points, const std::vector<unsigned short> &indices);
	
	// Draw a polygon with a texture placed on it
	// Using this method gives you the option of specifying texture indeces and coordinates
	// so you have more control over how the texture is placed into the shape
	void drawTexturedPolygon(const std::vector<Point> &points, Texture *tex, TextureFitOption fit, Alignment align);
	void drawTexturedPolygon(const std::vector<Point> &points, Texture *tex, const std::vector<Point> &texCoords);
	void drawTexturedPolygon(const std::vector<Point> &points, const std::vector<unsigned short> &indices, Texture *tex, const std::vector<Point> &texCoords);
	
	// drawPoints allows you to draw a shape and specify the GLenum type
	void drawPoints(const std::vector<Point> &points, GLenum type);
	void drawPoints(const std::vector<Point> &points, const std::vector<unsigned short> &indices, GLenum type);
	void drawPoints(const std::vector<Point> &points, Texture *tex, const std::vector<Point> &texCoords, GLenum type);
	void drawPoints(const std::vector<Point> &points, const std::vector<unsigned short> &indices, Texture *tex, const std::vector<Point> &texCoords, GLenum type);
    void drawPoints(Point* pt, int count, GLenum type);
	
	// The "generateStroke" method creates a very high quality stroke.
    // This stroke is actually drawn as a very thin filled shape. 
	// Given a list of points that define the corners of a certain shape, 
	// the method returns another list of points that can be used to draw the
	// stroke as a thin shape around the original shape.
	// For example you can draw a triangle as follows:
	//		std::vector<Point> shapePoints;
	//		shapePoints.push_back(Point(100,100));
	//		shapePoints.push_back(Point(300,100));
	//		shapePoints.push_back(Point(300,300));
	//		
	//		std::vector<Point> strokePoints = po::generateStroke(shapePoints, 10, true);
	//		po::drawPoints(strokePoints, GL_TRIANGLE_STRIP);
	
	std::vector<Point> generateStroke(std::vector<Point> &points, 
										int strokeWidth, 
										bool close = false,
										StrokePlacementProperty place = PO_STROKE_PLACE_CENTER, 
										StrokeJoinProperty join = PO_STROKE_JOIN_MITRE, 
										StrokeCapProperty cap = PO_STROKE_CAP_BUTT);
	
	// The "generateOval" method returns a list of points that can be used
	// to draw a 2D oval shape, given an horizontal and vertical radius.
	// The third argument specifies the number of points to be used.
	// For a smooth oval or circle, this should generally be about 1/3 the width or height of the shape.
	// For example, a 100x100 circle, should use approximately 33 points.
	// An oval can be drawn as follows:
	//		std::vector<Point> ovalPoints = po::generateOval(100, 100);
	//		po::drawPoints(ovalPoints, GL_TRIANGLE_FAN);
	
	std::vector<Point> generateOval(float xRad, float yRad, uint resolution=50);
    
    
    //Rounded Rect returns 
    std::vector<Point> roundedRect(float w, float h, float r);
    std::vector<Point> quadTo(Point p1, Point p2, Point control, int resolution);
    std::vector<Point> cubeTo(Point p1, Point p2, Point c1, Point c2, int resolution);
    float curveLength(const std::vector<Point> &curve);
} /*End po Namespace*/
