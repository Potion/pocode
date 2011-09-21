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
#include "poOpenGLState.h"

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
		default:
			return 0;
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
			*internal_format = GL_LUMINANCE;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_16:
			*format = GL_LUMINANCE_ALPHA;
			*internal_format = GL_LUMINANCE_ALPHA;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_24:
			*format = GL_RGB;
			*internal_format = GL_RGB;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_32:
			*format = GL_RGBA;
			*internal_format = GL_RGBA;
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
			return GL_LUMINANCE;
		case GL_ALPHA:
			return GL_ALPHA;
		case GL_LUMINANCE_ALPHA:
			return GL_LUMINANCE_ALPHA;
		case GL_RGB:
			return GL_RGB;
		case GL_RGBA:
			return GL_RGBA;
		default:
			log("poTexture: unsupported texture format (%s)\n", f);
			return GL_LUMINANCE;
	}
}

poTextureConfig::poTextureConfig()
:	format(GL_RGB)
,	internalFormat(GL_RGB)
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_NEAREST)
,	magFilter(GL_NEAREST)
#ifdef POTION_IOS
,	wrapS(GL_CLAMP_TO_EDGE)
,	wrapT(GL_CLAMP_TO_EDGE)
#else
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
#endif
{}

poTextureConfig::poTextureConfig(GLenum format)
:	format(format)
,	internalFormat(internalForFormat(format))
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_NEAREST)
,	magFilter(GL_NEAREST)
#ifdef POTION_IOS
,	wrapS(GL_CLAMP_TO_EDGE)
,	wrapT(GL_CLAMP_TO_EDGE)
#else
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
#endif
{}

poTexture::poTexture()
:	_image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{}

poTexture::poTexture(poImage *img)
:	_image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{
	if(img && img->isValid()) {
		_image = img;
		formatsForBitDepth(_image->bpp(), &config.format, &config.internalFormat, &config.type);
		load();
	}
}

poTexture::poTexture(poImage *img, poTextureConfig cfg)
:	_image(NULL)
,	uid(0)
,	_width(0)
,	_height(0)
{
	if(img && img->isValid()) {
		_image = img;
		config = cfg;
		load();
	}
}

poTexture::poTexture(uint width, uint height, poTextureConfig config)
:	_image(NULL)
,	uid(0)
,	config(config)
,	_width(width)
,	_height(height)
{
	load();
}

poTexture::~poTexture() {
	unload();
	
	if(_image)
		_image->tex = NULL;
}

bool poTexture::opaqueAtPoint(poPoint p) const {
	poColor color = colorAtPoint(p);
	return color.A > 0.0;
}

poColor poTexture::colorAtPoint(poPoint p) const {
	if(_image && _image->isValid())
		return _image->getPixel(p);
	return poColor();
}

uint poTexture::width() const {
	if(_image && _image->isValid())
		return _image->width();
	return _width;
}

uint poTexture::height() const {
	if(_image && _image->isValid())
		return _image->height();
	return _height;
}

void poTexture::load() {
	if(!isLoaded()) {
		float w, h;
		const ubyte *pix;
		
		if(_image != NULL && _image->isValid()) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#ifndef POTION_IOS
			// sucks that this doesn't exist with opengl es
			// we'll cross this bridge later if necessary
			// possible solution is at
			// http://stackoverflow.com/questions/205522/opengl-subtexturing/205569#205569
			glPixelStorei(GL_UNPACK_ROW_BYTES_APPLE, _image->pitch());
#endif			
			w = _image->width();
			h = _image->height();
			pix = _image->pixels();
			
		}
		else if(_width > 0 && _height > 0) {
			w = _width;
			h = _height;
			pix = NULL;
		}
		
		glGenTextures(1, &uid);
		glBindTexture(GL_TEXTURE_2D, uid);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
#ifdef POTION_IOS
		// we'll probably need to modify the default texture shader to deal with this
		// see this thread from cairo
		// http://lists.freedesktop.org/archives/cairo/2011-February/021715.html
#else
		float trans[] = {0.f, 0.f, 0.f, 0.f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);
#endif

		glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, w, h, 0, config.format, config.type, pix);
		glBindTexture(GL_TEXTURE_2D, 0);
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

poImage const *poTexture::image() const {
	return _image;
}

void poTexture::bind() {
	poOpenGLState::get()->bindTexture(this);
}

void poTexture::unbind() {
	poOpenGLState::get()->unbindTexture();
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




