#include "poSimpleDrawing.h"
#include "poTexture.h"

void applyColor(poColor color) {
	glColor4fv(&color.red);
}

void drawQuad(GLenum type, float l, float r, float b, float t) {
	GLfloat quad[4*3] = { 
		l, b, 0, 
		l, t, 0, 
		r, b, 0, 
		r, t, 0 
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quad);
	glDrawArrays(type, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawStroke(poRect rect) {
	drawStroke(rect.left, rect.right, rect.bottom, rect.top);
}

void drawStroke(float l, float r, float b, float t) {
	drawQuad(GL_LINE_LOOP,l,r,b,t);
}

void drawRect(poRect rect) {
	drawRect(rect.left, rect.right, rect.bottom, rect.top);
}

void drawRect(float l, float r, float b, float t) {
	drawQuad(GL_TRIANGLE_STRIP, l,r,b,t); 
}

void drawRect(poRect rect, poTexture* texture) {
	drawRect(rect.left, rect.right, rect.top, rect.bottom, texture);
}

void drawRect(float l, float r, float b, float t, poTexture* texture) {
	GLfloat quad[4*3] = { 
		l, b, 0, 
		l, t, 0, 
		r, b, 0, 
		r, t, 0 
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


