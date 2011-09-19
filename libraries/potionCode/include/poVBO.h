
#pragma once

class poVBO {
public:
	poVBO();
	poVBO(GLenum target);
	~poVBO();
	
	void bind();
	void unbind();
	
	void bufferData(int size, const void *data, GLenum usage);
	void bufferSubData(int offset, int size, const void *data);

	void *map(GLenum access);
	void unmap();
	
private:
	GLuint uid;
	GLenum target;
};