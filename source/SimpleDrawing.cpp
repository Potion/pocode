#include "SimpleDrawing.h"

#include "Helpers.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poOpenGLState.h"
#include "poBasicRenderer.h"
#include "poApplication.h"

#include "LineExtruder.h"
#include "poResourceLoader.h"

#include <cfloat>
#include <utf8.h>
#include <boost/assign/list_of.hpp>

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

void po::setColor(poColor color) {
	poOpenGLState::get()->color = color;
}

void po::setColor(poColor color, float alpha) {
	poOpenGLState::get()->color = poColor(color, alpha);
}

void po::fill() {
    po::bFill = true;
}

void po::noFill() {
    po::bFill = false;
}

void po::stroke(bool bSimpleStroke) {
    po::useSimpleStroke = bSimpleStroke;
    po::bStroke = true;
}

void po::noStroke() {
    po::bStroke = false;
}

void po::drawStroke(poRect rect) {
	drawStroke(rect.x, rect.y, rect.width, rect.height);
}

void po::drawStroke(float x, float y, float w, float h) {
	drawQuad(GL_LINE_LOOP,x,y,w,h);
}


std::vector<poPoint> po::generateStroke(std::vector<poPoint> &points, int strokeWidth, poStrokePlacementProperty place, poStrokeJoinProperty join, poStrokeCapProperty cap) {
    std::vector<poPoint> stroke;
    
    useSimpleStroke = false;
	stroke_width = strokeWidth;
    
    po::cap = cap;
    po::join = join;
	
    po::bStroke = stroke_width > 0;
	if(!bStroke){
        return stroke;
    }
	
    
	if(po::bStroke) {
		std::vector<poExtrudedLineSeg> segments;
		
		poPoint p1, p2, p3, p4, tmp;
		
		for(int i=0; i<points.size()-1; i++) {
			p1 = points[i];
			p2 = points[i+1];
			segments.push_back(poExtrudedLineSeg(p1, p2, stroke_width, place));
		}
		
		if(po::bClosed) {
			segments.push_back(poExtrudedLineSeg(points.back(), points.front(), stroke_width, place));
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, stroke_width);
		}	
		else {
			// add the first cap
			stroke.push_back(segments[0].p2);
			stroke.push_back(segments[0].p1);
		}
		
		// generate middle points
		for(int i=0; i<segments.size()-1; i++) {
			makeStrokeForJoint(stroke, segments[i], segments[i+1], join, stroke_width);
		}
		
		if(po::bClosed) {
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, stroke_width);
		}
		else {
			stroke.push_back(segments.back().p4);
			stroke.push_back(segments.back().p3);
		}
	}
	
	return stroke;
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

void po::drawRect(poRect rect) {
	drawRect(rect.x, rect.y, rect.width, rect.height);
}

void po::drawRect(float x, float y, float w, float h) {
	drawQuad(GL_TRIANGLE_STRIP, x,y,w,h); 
}

void po::drawRect(poTexture* tex) {
	drawRect(poRect(0,0,tex->width,tex->height), poRect(0,0,1,1), tex);
}

void po::drawRect(poRect rect, poTexture* texture) {
	drawRect(rect, poRect(0,0,1,1), texture);
}

void po::drawRect(float x, float y, float w, float h, poTexture* texture) {
	drawRect(poRect(x,y,w,h), poRect(0,0,1,1), texture);
}

void po::drawRect(poRect rect, poTexture *tex, poTextureFitOption fit) {
	std::vector<poPoint> coords = textureFit(rect, tex, fit, PO_ALIGN_TOP_LEFT);
	poRect coords_rect(coords[0], coords[2]);
	drawRect(rect, coords_rect, tex);
}

void po::drawRect(poRect rect, poRect coords, poTexture *texture) {
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
	ogl->setTexture(po::TextureState(texture));
	ogl->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void po::drawCircle(float x, float y, float radius) {
    po::drawOval(x, y, radius, radius);
    
}

void po::drawOval(float x, float y, float width, float height) {
    std::vector<poPoint>    points;
    
    float angleIncrement = M_2PI / (float) po::circleResolution;
    float theta = 0.0f;
    for( int i=0; i<po::circleResolution; i++ ) {
        points.push_back(poPoint(x + (width/2 * cos(theta)), y + (height/2 * sin(theta)), 0));
        theta += angleIncrement;
    }
    
    // do shape fill
	if (po::bFill) {
        po::drawPoints(GL_TRIANGLE_FAN, points);
	}
	
	// do shape stroke
	if(po::bStroke) {
        if(po::useSimpleStroke) {
            // use crappy OpenGL stroke
			glLineWidth( stroke_width );
			GLenum primitiveType = po::bClosed ? GL_LINE_LOOP : GL_LINE_STRIP;
			po::drawPoints(primitiveType, points);
        } else {
            po::drawPoints(GL_TRIANGLE_FAN, po::generateStroke(points, 1));
        }
	}
}

void po::drawPoints(GLenum type, const std::vector<poPoint> &points) {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setTexture(po::TextureState());
	ogl->setVertex(po::VertexState().enableAttrib(0).disableAttrib(1));

	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
}

void po::drawPoints(GLenum type, poTexture *texture, const std::vector<poPoint> &points, const std::vector<poPoint> &tex_coords) {
	poOpenGLState *ogl = poOpenGLState::get();
	
	ogl->setTexture(po::TextureState(texture));
	poOpenGLState::get()->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));
	
	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(poPoint), &tex_coords[0]);
	glDrawArrays(type, 0, points.size());
}

void drawPoints(GLenum type, const std::vector<poPoint> &points, poTexture *texture) {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setTexture(po::TextureState());
	ogl->setVertex(po::VertexState().enableAttrib(0).enableAttrib(1));

	poBasicRenderer::get()->setFromState();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
}

void po::drawString(const std::string &str, poFont *font, poPoint pos, int ptSize, float tracking) {
	if(ptSize > 0)
		font->pointSize(ptSize);
	
	poBitmapFont *bitmapFont = getBitmapFont(font, ptSize);

	font->glyph(' ');
	float spacer = font->glyphAdvance().x * tracking;
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		uint codepoint = utf8::next(ch, str.end());

		font->glyph(codepoint);
		poPoint adv = font->glyphAdvance();
		poPoint org = round(pos+font->glyphBearing());
		org.y += font->ascender();
		
		bitmapFont->drawUID( codepoint, org );
		
		pos.x += adv.x * tracking;
	}
}

void po::enableBlending() {
	BlendState blend;
	blend.enabled = true;
	blend.source_factor = GL_SRC_ALPHA;
	blend.dest_factor = GL_ONE_MINUS_SRC_ALPHA;

	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setBlend(blend);
}

void po::disableBlending() {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->setBlend(BlendState());
}


