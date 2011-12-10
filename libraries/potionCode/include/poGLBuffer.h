#pragma once

#include "common.h"

class poGLBufferConfig {
public:
	poGLBufferConfig();
	poGLBufferConfig(GLenum target);
	poGLBufferConfig(GLenum target, GLenum usage);
	
	// vbo: GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER
	// pbo: GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER
	GLenum target;
	
	// probably doesn't matter, but options are 
	// GL_{STATIC|DYNAMIC|STREAM}_{READ|DRAW|COPY}
	GLenum usage;
};

#define PO_BUFFER_OFFSET(i) ((char*)NULL + i)

class poGLBuffer {
public:
	poGLBuffer(GLenum target, size_t size);
	poGLBuffer(poGLBufferConfig config, size_t size);
	~poGLBuffer();
	
	void				bind();
	void				unbind();
	
	void				bufferData(size_t size, const void *data, GLenum usage);
	void				bufferSubData(size_t offset, size_t size, const void *data);

	void*				map(GLenum access);
	void				unmap();
	
	bool				isValid() const;
	uint				getUid() const;
	uint				getSize() const;
	poGLBufferConfig	getConfig() const;
	
	void				setSize(size_t sz);
	void				setConfig(poGLBufferConfig config);
	void				setTarget(GLenum target);
	void				setUsage(GLenum usage);
	
private:
	size_t size;
	GLuint uid;
	poGLBufferConfig config;
};