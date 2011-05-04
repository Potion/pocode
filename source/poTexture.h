//
//  poTexture.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poImage.h"

class poTexture 
	: public poResource
{
public:
	poTexture();
	poTexture(poImage *img);
	poTexture(GLenum format, uint width, uint height, uint num_bytes, ubyte const*pixels);
	poTexture(GLenum format, GLenum internal_Format, GLenum type,
			  uint width, uint height, uint num_bytes, ubyte const*pixels);
	~poTexture();
	
	poTexture *copy();

	uint uid() const;
	uint width() const;
	uint height() const;
	float s() const;
	float t() const;
	GLenum format() const;
	GLenum internalFormat() const;
	GLenum type() const;
	GLenum minFilter() const;
	GLenum magFilter() const;
	GLenum wrapS() const;
	GLenum wrapT() const;
	
	bool isOnCard() const;
	bool storingPixels() const;
	void deleteLocalMemory();
	void pushToCard();
	void pullFromCard();
	
	void bind(uint unit=0);
	void unbind(uint unit=0);
	
	void st(float s, float t);

protected:
	void createRefCounter();
	void incrRefCount();
	void decrRefCount();
	uint refCount();

private:
	void load(GLenum format, GLenum internal_format, GLenum type, 
			  GLenum min, GLenum mag, GLenum ws, GLenum wt,
			  uint w, uint h, uint mem, ubyte const*pixels);

	GLuint _uid;
	float _s, _t;
	uint _width, _height;
	GLenum _format, _internal_format, _type;
	GLenum _min_filter, _mag_filter;
	GLenum _wrap_s, _wrap_t;
	uint _mem_size;
	GLubyte *_pixels;
	uint *ref_count;
};


