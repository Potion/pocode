#include "SimpleDrawing.h"

#include "Helpers.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poOpenGLState.h"
#include "poBasicRenderer.h"
#include "poOpenGLStateChange.h"

#include <cfloat>
#include <utf8.h>
#include <boost/assign/list_of.hpp>

void setColor(poColor color) {
	poOpenGLState::get()->color = color;
}

void setColor(poColor color, float new_alpha) {
	setColor(poColor(color, new_alpha));
}

poColor currentColor() {
    return theColor;
}

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);

	poTextureState state(0);
	poBasicRenderer::get()->setFromState();
	
	glDrawArrays(type, 0, 4);
}

void drawStroke(poRect rect) {
	drawStroke(rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
}

void drawStroke(float x, float y, float w, float h) {
	drawQuad(GL_LINE_LOOP,x,y,w,h);
}

void drawLine(poPoint a, poPoint b) {
	a = floor(a) + poPoint(0.5f, 0.5f);
	b = floor(b) + poPoint(0.5f, 0.5f);
	
	GLfloat points[2*3] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};
	
	poTextureState state();
	poBasicRenderer::get()->setFromState();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, points);
	glDrawArrays(GL_LINES, 0, 2);
}

void drawRect(poRect rect) {
	drawRect(rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
}

void drawRect(float x, float y, float w, float h) {
	drawQuad(GL_TRIANGLE_STRIP, x,y,w,h); 
}

void drawRect(poRect rect, poTexture* texture, bool flip) {
	drawRect(rect, poRect(0,0,1,1), texture, flip);
}

void drawRect(float x, float y, float w, float h, poTexture* texture, bool flip) {
	drawRect(poRect(x,y,w,h), texture, flip);
}

void drawRect(poRect rect, poTexture *tex, poTextureFitOption fit) {
	std::vector<poPoint> coords = textureFit(rect, tex, fit, PO_ALIGN_TOP_LEFT);
	poRect coords_rect(coords[0], coords[2]);
	drawRect(rect, coords_rect, tex);
}

void drawRect(poRect rect, poRect coords, poTexture *texture, bool flip) {
	rect.origin = floor(rect.origin) + poPoint(.5f, .5f);
	
	GLfloat quad[4*3] = { 
		rect.origin.x, rect.origin.y, 0, 
		rect.origin.x, rect.origin.y+rect.size.y, 0, 
		rect.origin.x+rect.size.x, rect.origin.y, 0, 
		rect.origin.x+rect.size.x, rect.origin.y+rect.size.y, 0 
	};
	
	GLfloat tcoords[4*2] = {
		coords.origin.x, coords.origin.y,
		coords.origin.x, coords.origin.y+coords.size.y,
		coords.origin.x+coords.size.x, coords.origin.y,
		coords.origin.x+coords.size.x, coords.origin.y+coords.size.y
	};
	
	poTextureState texState(texture->uid);
	poBasicRenderer::get()->setFromState();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void drawRect(poTexture* tex, bool flip) {
	drawRect(0,0,tex->width(),tex->height(),tex, flip);
}

void drawPoints(GLenum type, const std::vector<poPoint> &points) {
	poTextureState texState();
	poBasicRenderer::get()->setFromState();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
}

void textureFitExact(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.origin.x / (float)rect.width();
	float yoff = rect.origin.y / (float)rect.height();
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width() - xoff;
		float t = points[i].y / rect.height() - yoff;
		coords[i].set(s,t,0.f);
	}
}


void textureFitNone(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.origin.x / (float)rect.width();
	float yoff = rect.origin.y / (float)rect.height();
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / tex->width() - xoff;
		float t = points[i].y / tex->height() - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}


void textureFitHorizontal(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.origin.x / (float)rect.width();
	float yoff = rect.origin.y / (float)rect.height();
	
	float new_w = rect.width();
	float new_h = new_w / (tex->width() / (float)tex->height());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width() - xoff;
		float t = points[i].y / new_h - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}

void textureFitVertical(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.origin.x / (float)rect.width();
	float yoff = rect.origin.y / (float)rect.height();
	
	float new_h = rect.height();
	float new_w = new_h / (tex->height() / (float)tex->width());
	
	poPoint max(FLT_MIN, FLT_MIN);

	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / new_w - xoff;
		float t = points[i].y / rect.height() - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}

	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}

std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	std::vector<poPoint> coords(4);
	std::vector<poPoint> points = rect.corners();
	textureFit(rect, tex, fit, align, coords, points);
	return coords;
}

void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	switch(fit) {
		case PO_TEX_FIT_NONE:
			textureFitNone(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_EXACT:
			textureFitExact(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_H:
			textureFitHorizontal(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_V:
			textureFitVertical(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_INSIDE: 
		{
			float new_h = rect.width() * (tex->height() / (float)tex->width());
			if(new_h > rect.height())
				textureFitVertical(rect, tex, align, coords, points);
			else
				textureFitHorizontal(rect, tex, align, coords, points);
			break;
		}
		default:
			;
	}
}

void drawString(const std::string &str, poFont *font, poPoint pos, int ptSize, float tracking) {
	if(ptSize > 0)
		font->pointSize(ptSize);
	
	poBitmapFont *bitmapFont = getBitmapFont(font);

	font->glyph(' ');
	float spacer = font->glyphAdvance().x * tracking;

	bitmapFont->setUpFont();
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		uint codepoint = utf8::next(ch, str.end());
		font->glyph(codepoint);
		
		poPoint adv = font->glyphAdvance();
		poPoint org = round(pos+font->glyphBearing());
		
		bitmapFont->drawGlyph( codepoint, pos+font->glyphBearing() );
		
		pos.x += adv.x * tracking;
	}
	
	bitmapFont->setDownFont();
}


