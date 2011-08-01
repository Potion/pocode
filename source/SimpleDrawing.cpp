#include "SimpleDrawing.h"

#include "Helpers.h"
#include "poTexture.h"

#include <cfloat>
#include <boost/assign/list_of.hpp>


void applyColor(poColor color) {
	glColor4fv(&color.R);
}

void drawQuad(GLenum type, float x, float y, float w, float h) {
	GLfloat quad[4*3] = {
		x, y, 0, 
		x, y+h, 0, 
		x+w, y, 0, 
		x+w, y+h, 0 
	};
	
	if(type == GL_LINE_STRIP || type == GL_LINE_LOOP)
		std::swap(quad[7], quad[10]);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quad);
	glDrawArrays(type, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawStroke(poRect rect) {
	drawStroke(rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
}

void drawStroke(float x, float y, float w, float h) {
	drawQuad(GL_LINE_LOOP,x,y,w,h);
}

void drawLine(poPoint a, poPoint b) {
	GLfloat points[2*3] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, points);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	
	if(flip) {
//		int row_sz = 2*sizeof(GLfloat);
//		
//		GLfloat tmp[2];
//		memcpy(tmp, tcoords+row_sz, row_sz);
//		memcpy(tcoords+row_sz, tcoords+row_sz*2, row_sz);
//		memcpy(tcoords+row_sz*2, tcoords+row_sz*3, row_sz);
//		memcpy(tcoords+row_sz*3, tmp, row_sz);
	}
	
	glPushAttrib(GL_TEXTURE_BIT);
	glPushClientAttrib(GL_TEXTURE_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, quad);
	glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
	
	texture->bind(0);
	
	glEnable(GL_TEXTURE_2D);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPopClientAttrib();
	glPopAttrib();
}

void drawRect(poTexture* tex, bool flip) {
	drawRect(0,0,tex->width(),tex->height(),tex, flip);
}

void drawPoints(GLenum type, const std::vector<poPoint> &points) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
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
			float new_h = rect.width() * (tex->height() / (float)tex->width());
			
			if(new_h > rect.height())
				textureFitVertical(rect, tex, align, coords, points);
			else
				textureFitHorizontal(rect, tex, align, coords, points);
			
			break;
	}
}



