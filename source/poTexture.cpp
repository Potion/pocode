//
//  poTexture.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTexture.h"

void formatsForBitDepth(ImageBitDepth bpp, GLenum *format, GLenum *internal_format, GLenum *type) {
	switch(bpp) {
		case IMAGE_8:
			*format = GL_LUMINANCE;
			*internal_format = GL_LUMINANCE;
			*type = GL_UNSIGNED_BYTE;
			break;
			
		case IMAGE_16:
			*format = GL_BGR;
			*internal_format = 3;
			*type = GL_UNSIGNED_SHORT_5_6_5;
			break;
			
		case IMAGE_24:
			*format = GL_BGR;
			*internal_format = 3;
			*type = GL_UNSIGNED_BYTE;
			break;
			
		case IMAGE_32:
			*format = GL_BGRA;
			*internal_format = 4;
			*type = GL_UNSIGNED_BYTE;
			break;
			
		default:
			printf("UNKNOW IMAGE TYPE\n");
	}
}

size_t bytesForPixelFormat(GLenum format) {
	switch(format) {
		case GL_BGRA:
		case GL_RGBA:	return 4; break;
			
		case GL_BGR:
		case GL_RGB:	return 3; break;
			
		case GL_ALPHA:
		case GL_LUMINANCE: return 1; break;
			
		default:		return 0;
	}
}

poTexture::poTexture() {}

poTexture::poTexture(poImage *img) {
	load(img);
}

poTexture::poTexture(const std::string &str) {
	poImage *img = new poImage(str);
	load(img);
	delete img;
}

poTexture::poTexture(GLenum format, uint width, uint height, uint mem, ubyte const*pixels) {
	load(format, format, GL_UNSIGNED_BYTE, 
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 width, height, mem, pixels);
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

poTexture::poTexture(GLenum format, GLenum internal_format, GLenum type,
					 uint width, uint height, uint mem, ubyte const*pixels)
{
	load(format, internal_format, type,
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 width, height, mem, pixels);
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

poTexture::~poTexture() {
	decrRefCount();
	if(refCount() <= 0) {
		if(isOnCard())
			pullFromCard();
		if(storingPixels())
			deleteLocalMemory();
		delete ref_count;
	}
}

poTexture *poTexture::copy() {
	poTexture *tex = new poTexture();
	tex->load(_format, _internal_format, _type, 
			  _min_filter, _mag_filter, _wrap_s, _wrap_t,
			  _width, _height, 0, NULL);
	tex->st(_s, _t);
	tex->_uid = _uid;
	
	tex->ref_count = ref_count;
	incrRefCount();
	
	return tex;
}

uint poTexture::uid() const			{return _uid;}
float poTexture::width() const		{return _width;}
float poTexture::height() const		{return _height;}
float poTexture::s() const			{return _s;}
float poTexture::t() const			{return _t;}
GLenum poTexture::format() const	{return _format;}
GLenum poTexture::internalFormat() const {return _internal_format;}
GLenum poTexture::type() const		{return _type;}
GLenum poTexture::minFilter() const {return _min_filter;}
GLenum poTexture::magFilter() const {return _mag_filter;}
GLenum poTexture::wrapS() const		{return _wrap_s;}
GLenum poTexture::wrapT() const		{return _wrap_t;}

bool poTexture::isOnCard() const {return _uid != 0;}
bool poTexture::storingPixels() const {return _pixels != NULL;}
void poTexture::deleteLocalMemory() {
	delete [] _pixels; 
	_pixels = NULL; 
	_mem_size = 0;
}

void poTexture::pushToCard() {
	if(storingPixels() && !isOnCard()) {
		glGenTextures(1, &_uid);
		glBindTexture(GL_TEXTURE_2D, _uid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
		glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _format, _type, _pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void poTexture::pullFromCard() {
	if(isOnCard()) {
		glDeleteTextures(1, &_uid);
		_uid = 0;
	}
}

void poTexture::bind(uint unit) {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, _uid);
}

void poTexture::unbind(uint unit) {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void poTexture::st(float s, float t) {
	_s = s;
	_t = t;
}

void poTexture::createRefCounter() {
	ref_count = new uint[1];
	*ref_count = 0;
}

void poTexture::incrRefCount() {
	(*ref_count)++;
}

void poTexture::decrRefCount() {
	(*ref_count)--;
}

uint poTexture::refCount() {
	return *ref_count;
}

void poTexture::load(poImage *img) {
	GLenum f, i, t; 
	formatsForBitDepth(img->bpp(), &f, &i, &t);
	
	load(f, i, t,
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 img->width(), img->height(), img->storageSize(), img->pixels());
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

void poTexture::load(GLenum format, GLenum internal_format, GLenum type, 
					 GLenum min, GLenum mag, GLenum ws, GLenum wt,
					 uint w, uint h, uint mem, ubyte const*pixels)
{
	_uid = 0;
	_s = _t = 1.f;
	_format = format;
	_internal_format = internal_format;
	_type = type;
	_min_filter = min;
	_mag_filter = mag;
	_wrap_s = ws;
	_wrap_t = wt;
	_width = w;
	_height = h;
	_mem_size = mem;
	_pixels = new ubyte[_mem_size];
	memcpy(_pixels, pixels, _mem_size);
}



