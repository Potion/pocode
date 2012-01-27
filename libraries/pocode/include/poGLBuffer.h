/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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