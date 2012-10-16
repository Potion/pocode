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

#include "poSimpleDrawing.h"

#include "poHelpers.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poOpenGLState.h"
#include "poApplication.h"

#include "poLineExtruder.h"
#include "poResourceStore.h"

#include <cfloat>
#include <utf8.h>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

void po::drawLine(poPoint a, poPoint b) {
	a = floor(a) + poPoint(0.5f, 0.5f);
	b = floor(b) + poPoint(0.5f, 0.5f);
	
	GLfloat points[2*3] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};
	
	use2DShader();
	updateActiveShader();
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, points);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(0);
}

void po::drawString(const std::string &str, poFont *font, poPoint pos, int ptSize, float tracking) {
	if(ptSize > 0)
		font->setPointSize(ptSize);
	
	font->setGlyph(' ');
	float spacer = font->getGlyphAdvance().x * tracking;
	
	poBitmapFont *bmpFont = poGetBitmapFont(font, ptSize);
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		uint codepoint = utf8::next(ch, str.end());
		
		font->setGlyph(codepoint);
		poPoint adv = font->getGlyphAdvance();
		poPoint org = round(pos+font->getGlyphBearing());
		org.y += font->getAscender();
		
		bmpFont->drawGlyph( codepoint, org );
		
		pos.x += adv.x * tracking;
	}
}

namespace {
	void drawQuad(GLenum type, float x, float y, float w, float h) {
		x = floor(x) + .5f;
		y = floor(y) + .5f;
		w = floor(w);
		h = floor(h);
		
		GLfloat quad[4*3] = {
			x, y, 0, 
			x, y+h, 0, 
			x+w, y, 0, 
			x+w, y+h, 0 
		};
		
		if(type == GL_LINE_STRIP || type == GL_LINE_LOOP)
			std::swap(quad[7], quad[10]);
		
		po::use2DShader();
		po::updateActiveShader();
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
		glDrawArrays(type, 0, 4);
		glDisableVertexAttribArray(0);
	}
}

void po::drawStrokedRect(float x, float y, float w, float h) {
	drawQuad(GL_LINE_LOOP,x,y,w,h);
}

void po::drawStrokedRect(poRect rect) {
	drawStrokedRect(rect.x, rect.y, rect.width, rect.height);
}

void po::drawFilledRect(float x, float y, float w, float h) {
	drawQuad(GL_TRIANGLE_STRIP, x,y,w,h); 
}

void po::drawFilledRect(poRect rect) {
	drawFilledRect(rect.x, rect.y, rect.width, rect.height);
}

void po::drawFilledCircle(float x, float y, float rad) {
	int segs = 10.f * sqrtf(rad);
	float theta = 2.f * M_PI / (float)segs;
	float tan_t = tanf(theta);
	float cos_t = cosf(theta);

	float x1 = rad;
	float y1 = 0.f;

	std::vector<poPoint> pt;
	for(int i=0; i<segs; ++i) {
		pt.push_back(poPoint(x1+x, y1+y));

		float tx = -y1;
		float ty = x1;

		x1 = (x1 + tx * tan_t) * cos_t;
		y1 = (y1 + ty * tan_t) * cos_t;
	}
	
	drawPoints(pt, GL_TRIANGLE_FAN);
}

void po::drawTexturedRect(poTexture *tex) {
	drawTexturedRect(tex, poRect(0,0,tex->getWidth(),tex->getHeight()), poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, float x, float y, float w, float h) {
	drawTexturedRect(tex, poRect(x,y,w,h), poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, poRect rect) {
	drawTexturedRect(tex, rect, poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, poRect rect, poTextureFitOption fit, poAlignment align) {
	std::vector<poPoint> coords = textureFit(rect, tex, fit, align);
	poRect coords_rect(coords[0], coords[2]);
	drawTexturedRect(tex, rect, coords_rect);
}

void po::drawTexturedRect(poTexture *tex, poRect rect, poRect coords) {
	GLfloat quad[4*3] = {
		rect.x,  rect.y, 0, 
		rect.x+rect.width, rect.y, 0, 
		rect.x+rect.width, rect.y+rect.height, 0,
		rect.x,  rect.y+rect.height, 0,
	};
	
	GLfloat tcoords[4*2] = {
		coords.x, coords.y+coords.height,
		coords.x+coords.width, coords.y+coords.height,
		coords.x+coords.width, coords.y,
		coords.x, coords.y,
	};
	
	useTexture(tex->getUid(), tex->hasAlpha());
	
	if(tex->getConfig().internalFormat == GL_ALPHA)
		useTex2DMaskShader();
	else
		useTex2DShader();
	
	updateActiveShader();
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void po::drawStrokedPolygon(const std::vector<poPoint> &points) {
	drawPoints(points, GL_LINE_LOOP);
}

void po::drawStrokedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices) {
	drawPoints(points, indices, GL_LINE_LOOP);
}

void po::drawFilledPolygon(const std::vector<poPoint> &points) {
	drawPoints(points, GL_TRIANGLE_FAN);
}

void po::drawFilledPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices) {
	drawPoints(points, indices, GL_TRIANGLE_FAN);
}

void po::drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords) {
	drawPoints(points, tex, texCoords, GL_TRIANGLE_FAN);
}

void po::drawTexturedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords) {
	drawPoints(points, indices, tex, texCoords, GL_TRIANGLE_FAN);
}

void po::drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	if(points.empty())
		return;

	poRect bounds( points[0].x, points[0].y, 0, 0 );
    BOOST_FOREACH(poPoint p, points) {
        bounds.include(p);
    }
	
	std::vector<poPoint> coords(points.size());
	textureFit(bounds, tex, fit, align, coords, points);
	po::drawTexturedPolygon(points, tex, coords);
}

void po::drawPoints(const std::vector<poPoint> &points, GLenum type) {
	use2DShader();
	updateActiveShader();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
	glDisableVertexAttribArray(0);
}

void po::drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type) {
	use2DShader();
	updateActiveShader();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawElements(type, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableVertexAttribArray(0);
}

void po::drawPoints(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type) {
	useTexture(tex->getUid(), tex->hasAlpha());
	
	useTex2DShader();
	updateActiveShader();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(poPoint), &texCoords[0]);
	glDrawArrays(type, 0, points.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void po::drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type) {
	useTexture(tex->getUid(), tex->hasAlpha());
	
	useTex2DShader();
	updateActiveShader();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(poPoint), &texCoords[0]);
	glDrawElements(type, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void po::drawPoints(poPoint* pt, int count, GLenum type) {
	use2DShader();
	updateActiveShader();
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &pt[0].x);
	glDrawArrays(type, 0, count);
	glDisableVertexAttribArray(0);
}

std::vector<poPoint> po::generateStroke(std::vector<poPoint> &points, 
										int strokeWidth, 
										bool close, 
										poStrokePlacementProperty place, 
										poStrokeJoinProperty join, 
										poStrokeCapProperty cap) 
{
    std::vector<poPoint> stroke;
	
	std::vector<poExtrudedLineSeg> segments;
	poPoint p1, p2, p3, p4, tmp;
	
	for(uint i=0; i<points.size()-1; i++) {
		p1 = points[i];
		p2 = points[i+1];
		segments.push_back(poExtrudedLineSeg(p1, p2, strokeWidth, place));
	}
	
	if(close) {
		segments.push_back(poExtrudedLineSeg(points.back(), points.front(), strokeWidth, place));
		makeStrokeForJoint(stroke, segments.back(), segments.front(), join, strokeWidth);
	}	
	else {
		// add the first cap
		stroke.push_back(segments[0].p2);
		stroke.push_back(segments[0].p1);
	}
	
	// generate middle points
	for(uint i=0; i<segments.size()-1; i++) {
		makeStrokeForJoint(stroke, segments[i], segments[i+1], join, strokeWidth);
	}
	
	if(close) {
		makeStrokeForJoint(stroke, segments.back(), segments.front(), join, strokeWidth);
	}
	else {
		stroke.push_back(segments.back().p4);
		stroke.push_back(segments.back().p3);
	}
	
	return stroke;
}

std::vector<poPoint> po::generateOval(float xRad, float yRad, uint resolution) {
	std::vector<poPoint> response;
	
	for(uint i=0; i<resolution; i++) {
		float phase = (i / float(resolution)) * M_2PI;
		response.push_back(poPoint(cosf(phase)*xRad, sinf(phase)*yRad, 0.f));
	}
	
	return response;
}
