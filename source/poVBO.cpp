#include "poVBO.h"

#ifdef POTION_IOS
	#define glMapBuffer glMapBufferOES
	#define glUnmapBuffer glUnmapBufferOES
#endif

poVBO::poVBO()
:	target(GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &uid);
}

poVBO::poVBO(GLenum target)
:	target(target)
{
	glGenBuffers(1, &uid);
}

poVBO::~poVBO() {
	glDeleteBuffers(1, &uid);
}

void poVBO::bind() {
	glBindBuffer(target, uid);
}

void poVBO::unbind() {
	glBindBuffer(target, 0);
}

void poVBO::bufferData(int size, const void *data, GLenum usage) {
	glBufferData(target, size, data, usage);
}

void poVBO::bufferSubData(int offset, int size, const void *data) {
	glBufferSubData(target, offset, size, data);
}

void *poVBO::map(GLenum access) {
	return glMapBuffer(target, access);
}

void poVBO::unmap() {
	glUnmapBuffer(target);
}
