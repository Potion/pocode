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
			*internal_format = 1;
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

poTexture::poTexture() {}

poTexture::poTexture(poImage img) {
	GLenum f, i, t; 
	formatsForBitDepth(img.bpp(), &f, &i, &t);
	reference = boost::shared_ptr<texture_impl>(new texture_impl(f, i, t,
																 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
																 img.width(), img.height(), img.pixels()));
}

poTexture::poTexture(const poTexture &tex) {
	reference = tex.reference;
}

poTexture &poTexture::operator=(const poTexture &tex) {
	reference = tex.reference;
	return *this;
}

uint poTexture::uid() const			{return reference->uid;}
uint poTexture::width() const		{return reference->width;}
uint poTexture::height() const		{return reference->height;}
float poTexture::s() const			{return reference->s;}
float poTexture::t() const			{return reference->t;}
GLenum poTexture::format() const	{return reference->format;}
GLenum poTexture::internalFormat() const {return reference->internal_format;}
GLenum poTexture::type() const		{return reference->type;}
GLenum poTexture::minFilter() const {return reference->min_filter;}
GLenum poTexture::magFilter() const {return reference->mag_filter;}
GLenum poTexture::wrapS() const		{return reference->wrap_s;}
GLenum poTexture::wrapT() const		{return reference->wrap_t;}

void poTexture::bind(uint unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, reference->uid);
}

void poTexture::unbind(uint unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void poTexture::st(float s, float t) {
	reference->s = s;
	reference->t = t;
}

poTexture::texture_impl::texture_impl(GLenum format, GLenum internal_format, GLenum type, 
									  GLenum min, GLenum mag, GLenum ws, GLenum wt,
									  int w, int h, ubyte const*pixels)
{
	s = t = 1.f;
	this->format = format;
	this->internal_format = internal_format;
	this->type = type;
	min_filter = min;
	mag_filter = mag;
	wrap_s = ws;
	wrap_t = wt;
	width = w;
	height = h;

	glGenTextures(1, &uid);
	glBindTexture(GL_TEXTURE_2D, uid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

poTexture::texture_impl::~texture_impl() {
	glDeleteTextures(1, &uid);
	printf("delete tex\n");
}



