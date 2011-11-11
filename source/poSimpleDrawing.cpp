#include "poSimpleDrawing.h"

#include "poHelpers.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poOpenGLState.h"
#include "poBasicRenderer.h"
#include "poApplication.h"

#include "poLineExtruder.h"
#include "poResourceStore.h"

#include <cfloat>
#include <utf8.h>
#include <boost/assign/list_of.hpp>

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
	
	for(int i=0; i<points.size()-1; i++) {
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
	for(int i=0; i<segments.size()-1; i++) {
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

	for(int i=0; i<resolution; i++) {
		float phase = i / float(resolution);
		response.push_back(poPoint(cosf(phase)*xRad, sinf(phase)*yRad, 0.f));
	}
	
	return response;
}

void po::setColor(poColor color) {
	poOpenGLState::get()->color = color;
}

void po::setColor(poColor color, float alpha) {
	poOpenGLState::get()->color = poColor(color, alpha);
}

void po::drawLine(poPoint a, poPoint b) {
	a = floor(a) + poPoint(0.5f, 0.5f);
	b = floor(b) + poPoint(0.5f, 0.5f);
	
	GLfloat points[2*3] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};
	
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setTexture(po::TextureState());
	ogl->setVertex(po::VertexState().enableAttrib(0).disableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, points);
	glDrawArrays(GL_LINES, 0, 2);
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
		
		poOpenGLState *ogl = poOpenGLState::get();
		ogl->setTexture(po::TextureState());
		ogl->setVertex(po::VertexState().enableAttrib(0).disableAttrib(1));
		
		poBasicRenderer::get()->setFromState();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
		glDrawArrays(type, 0, 4);
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

void po::drawTexturedRect(poTexture *tex) {
	drawTexturedRect(tex, poRect(0,0,tex->getWidth(),tex->getHeight()), poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, float x, float y, float w, float h) {
	drawTexturedRect(tex, poRect(x,y,w,h), poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, poRect rect) {
	drawTexturedRect(tex, rect, poRect(0,0,1,1));
}

void po::drawTexturedRect(poTexture *tex, poRect rect, poTextureFitOption fit) {
	std::vector<poPoint> coords = textureFit(rect, tex, fit, PO_ALIGN_TOP_LEFT);
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
	
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setTexture(po::TextureState(tex));
	ogl->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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

void po::drawPoints(const std::vector<poPoint> &points, GLenum type) {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setTexture(po::TextureState());
	ogl->setVertex(po::VertexState().enableAttrib(0).disableAttrib(1));

	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
}

void po::drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type) {
	poOpenGLState *ogl = poOpenGLState::get();
	
	ogl->setTexture(po::TextureState());
	poOpenGLState::get()->setVertex(po::VertexState().enableAttrib(0).disableAttrib(1));

	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawElements(type, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
}

void po::drawPoints(const std::vector<poPoint> &points, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type) {
	poOpenGLState *ogl = poOpenGLState::get();
	
	ogl->setTexture(po::TextureState(tex));
	poOpenGLState::get()->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(poPoint), &texCoords[0]);
	glDrawArrays(type, 0, points.size());
}

void po::drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture *tex, const std::vector<poPoint> &texCoords, GLenum type) {
	poOpenGLState *ogl = poOpenGLState::get();
	
	ogl->setTexture(po::TextureState(tex));
	poOpenGLState::get()->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(poPoint), &texCoords[0]);
	glDrawElements(type, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
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
