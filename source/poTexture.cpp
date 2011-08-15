//
//  poTexture.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTexture.h"
#include "poImage.h"
#include "Helpers.h"

GLenum formatForBitDepth(ImageBitDepth bpp) {
	switch(bpp) {
		case IMAGE_8:
			return GL_LUMINANCE;
			
		case IMAGE_16:
			return GL_LUMINANCE_ALPHA;
			
		case IMAGE_24:
			return GL_RGB;
			
		case IMAGE_32:
			return GL_RGBA;
	}
	
	return GL_LUMINANCE;
}

ImageBitDepth bitDepthForFormat(GLenum format) {
	switch(format) {
		case GL_LUMINANCE:
		case GL_ALPHA:
			return IMAGE_8;
			
		case GL_LUMINANCE_ALPHA:
			return IMAGE_16;
			
		case GL_RGB:
			return IMAGE_24;
			
		case GL_RGBA:
			return IMAGE_32;
	}
	
	return IMAGE_8;
}

bool formatsForBitDepth(ImageBitDepth bpp, GLenum *format, GLenum *internal_format, GLenum *type) {
	switch(bpp) {
		case IMAGE_8:
			*format = GL_LUMINANCE;
			*internal_format = GL_LUMINANCE8;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_16:
			*format = GL_LUMINANCE8_ALPHA8;
			*internal_format = 2;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_24:
			*format = GL_RGB;
			*internal_format = GL_RGB8;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_32:
			*format = GL_RGBA;
			*internal_format = GL_RGBA8;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		default:
			printf("poTexture: unsupported image bit depth (%d)\n", bpp);
			return false;
	}
}

size_t bytesForPixelFormat(GLenum format) {
	switch(format) {
		case GL_RGBA:	
			return 4; break;
			
		case GL_RGB:	
			return 3; break;
		
		case GL_LUMINANCE_ALPHA:	
			return 2; break;
			
		case GL_ALPHA:
		case GL_LUMINANCE: 
			return 1; break;
			
		default:		
			return 0;
	}
}

GLenum internalForFormat(GLenum f) {
	switch(f) {
		case GL_LUMINANCE:
			return GL_LUMINANCE8;
		case GL_ALPHA:
			return GL_ALPHA8;
		case GL_LUMINANCE_ALPHA:
			return GL_LUMINANCE8_ALPHA8;
		case GL_RGB:
			return GL_RGB8;
		case GL_RGBA:
			return GL_RGBA8;
		default:
			log("poTexture: unsupported texture format (%s)\n", f);
			return GL_LUMINANCE8;
	}
}

poTextureConfig::poTextureConfig()
:	format(GL_RGB)
,	internalFormat(GL_RGB)
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_NEAREST)
,	magFilter(GL_NEAREST)
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
{}

poTextureConfig::poTextureConfig(GLenum format)
:	format(format)
,	internalFormat(internalForFormat(format))
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_NEAREST)
,	magFilter(GL_NEAREST)
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
{}

poTexture::poTexture()
:	image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{}

poTexture::poTexture(poImage *img)
:	image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{
	if(img && *img) {
		image = img;
		formatsForBitDepth(image->bpp(), &config.format, &config.internalFormat, &config.type);
		load();
	}
}

poTexture::poTexture(poImage *img, poTextureConfig cfg)
:	image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{
	if(img && *img) {
		image = img;
		config = cfg;
		load();
	}
}

poTexture::poTexture(uint width, uint height, poTextureConfig config)
:	image(NULL)
,	uid(0)
,	config(config)
,	_width(width)
,	_height(height)
{
	load();
}

poTexture::~poTexture() {
	unload();
	
	if(image)
		image->tex = NULL;
}

bool poTexture::opaqueAtPoint(poPoint p) const {
	poColor color = colorAtPoint(p);
	return color.A > 0;
}

poColor poTexture::colorAtPoint(poPoint p) const {
	if(image && *image)
		return image->getPixel(p);
	return poColor();
}

uint poTexture::width() const {
	if(image)
		return image->width();
	return _width;
}

uint poTexture::height() const {
	if(image)
		return image->height();
	return _height;
}

void poTexture::load() {
	if(!isLoaded()) {
		if(image != NULL && image) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_BYTES_APPLE, image->pitch());
			
			glGenTextures(1, &uid);
			glBindTexture(GL_TEXTURE_2D, uid);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
			float trans[] = {0.f, 0.f, 0.f, 0.f};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);

			glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, image->width(), image->height(), 0, config.format, config.type, image->pixels());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if(_width > 0 && _height > 0) {
			glGenTextures(1, &uid);
			glBindTexture(GL_TEXTURE_2D, uid);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
			float trans[] = {0.f, 0.f, 0.f, 0.f};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);
			
			glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, _width, _height, 0, config.format, config.type, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void poTexture::unload() {
	if(isLoaded()) {
		glDeleteTextures(1, &uid);
		uid = 0;
	}
}

bool poTexture::isLoaded() const {
	return uid > 0;
}

void poTexture::bind(uint unit) const {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, uid);
}

void poTexture::unbind(uint unit) const {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//void loadNotFound() {
//	static poImage *img = NULL;
//	if(!img) {
//		ubyte *pix = new ubyte[10*10*3]();
//		for(int i=0; i<10*10; i++) {
//			pix[i*3+2] = 255;
//			pix[i*3+1] = 0;
//			pix[i*3] = 0;
//		}
//		img = new poImage(10,10,IMAGE_24,pix);
//	}
//	load(img);
//}
//
//




