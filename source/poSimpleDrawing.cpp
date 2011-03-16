#include "poSimpleDrawing.h"

void applyColor(poColor color) {
	glColor4fv(&color.red);
}

void drawRect(poRect rect) {
	drawRect(rect.left, rect.right, rect.top, rect.bottom);
}

void drawRect(float l, float r, float b, float t) {
	GLfloat quad[4*3] = { l, b, 0, 
		l, t, 0, 
		r, b, 0, 
		r, t, 0 
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quad);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}
