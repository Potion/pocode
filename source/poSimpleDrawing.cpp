#include "poSimpleDrawing.h"
#include "poTexture.h"

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

void drawRect(poRect rect) {
	drawRect(rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
}

void drawRect(float x, float y, float w, float h) {
	drawQuad(GL_TRIANGLE_STRIP, x,y,w,h); 
}

void drawRect(poRect rect, poTexture* texture) {
	drawRect(rect.origin.x, rect.origin.y, rect.size.x, rect.size.y, texture);
}

void drawRect(float x, float y, float w, float h, poTexture* texture) {
	GLfloat quad[4*3] = { 
		x, y, 0, 
		x, y+h, 0, 
		x+w, y, 0, 
		x+w, y+h, 0 
	};
	
	GLfloat tcoords[4*2] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
	
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

void drawRect(poTexture* tex) {
	drawRect(0,0,tex->width(),tex->height(),tex);
}

void drawPoints(const std::vector<poPoint> &points) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &points[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}
