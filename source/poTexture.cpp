//
//  poTexture.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTexture.h"
#include "poImage.h"
#include "poHelpers.h"
#include "poOpenGLState.h"
#include "poGLBuffer.h"


GLenum formatForChannels(uint channels) {
	switch(channels) {
		case 1:
			return GL_LUMINANCE;
		case 2:
			return GL_LUMINANCE_ALPHA;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		default:
			return 0;
	}
	return GL_LUMINANCE;
}

uint channelsForFormat(GLenum format) {
	switch(format) {
        #ifndef OPENGL_ES
		case GL_COLOR_INDEX:
		case GL_RED:
		case GL_GREEN:
		case GL_BLUE:
        #endif
		case GL_ALPHA:
		case GL_LUMINANCE:
			return 1;
		case GL_LUMINANCE_ALPHA:
			return 2;
		case GL_RGB:
        #ifndef OPENGL_ES
		case GL_BGR:
        #endif
			return 3;
		case GL_RGBA:
		case GL_BGRA:
			return 4;
	}
	return 1;
}

uint bitsPerChannelForType(GLenum type) {
	switch(type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
            
        #ifndef OPENGL_ES
		case GL_BITMAP:
		case GL_UNSIGNED_BYTE_3_3_2:
		case GL_UNSIGNED_BYTE_2_3_3_REV:
        #endif
			return 8;
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_UNSIGNED_SHORT_5_6_5:
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		case GL_UNSIGNED_SHORT_5_5_5_1:
		case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        #ifndef OPENGL_ES
		case GL_UNSIGNED_SHORT_5_6_5_REV:
        #endif
			return 16;
		case GL_INT:
		case GL_UNSIGNED_INT:
        #ifndef OPENGL_ES
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV:
		case GL_UNSIGNED_INT_10_10_10_2:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
        #endif
			return 32;
		case GL_FLOAT:
			return 32;
	}

	return 0;
}

uint bppForFormatAndType(GLenum format, GLenum type) {
	return channelsForFormat(format) * bitsPerChannelForType(type);
}

poTextureConfig::poTextureConfig()
:	format(GL_RGB)
,	internalFormat(GL_RGB)
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_LINEAR)
,	magFilter(GL_LINEAR)
	#if defined(OPENGL_ES)
,	wrapS(GL_CLAMP_TO_EDGE)
,	wrapT(GL_CLAMP_TO_EDGE)
	#else
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
	#endif
{}

poTextureConfig::poTextureConfig(GLenum format)
:	format(format)
,	internalFormat(format)
,	type(GL_UNSIGNED_BYTE)
,	minFilter(GL_LINEAR)
,	magFilter(GL_LINEAR)
	#if defined(OPENGL_ES)
,	wrapS(GL_CLAMP_TO_EDGE)
,	wrapT(GL_CLAMP_TO_EDGE)
	#else
,	wrapS(GL_CLAMP_TO_BORDER)
,	wrapT(GL_CLAMP_TO_BORDER)
	#endif
{}

int         poTexture::totalAllocatedTextureMemorySize = 0;

poTexture::poTexture()
:	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL)
{}

poTexture::poTexture(const std::string &url, bool keepImage )
:	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL)
{
	poImage* img = new poImage(url);
	load(img);
    
    if ( keepImage )
        sourceImage = img;
    else
        delete img;
}

poTexture::poTexture(poImage* img) 
:	uid(0), width(0), height(0), channels(0), config(), sourceImage(img)
{
	load(img);
}

poTexture::poTexture(poImage* img, const poTextureConfig &config)
:	uid(0), width(0), height(0), channels(0), config(), sourceImage(img)
{
	load(img, config);
}

poTexture::poTexture(uint width, uint height, const ubyte *pixels, const poTextureConfig &config)
:	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL)
{
	load(width, height, pixels, config);
}

poTexture::~poTexture() {
	glDeleteTextures(1, &uid);
	uid = 0;
    
    totalAllocatedTextureMemorySize -= width*height*channels;
    
    // should delete sourceImage in some cases
}

poTexture* poTexture::copy() {
	#if defined(OPENGL_ES)
	#warning poTexture::copy not implemented on iOS\n\
	#warning call poFBO::copyColorTexture to copy out of an FBO\n
		return NULL;
	#else
		glBindTexture(GL_TEXTURE_2D, uid);
		
		#ifndef OPENGL_ES
		poGLBuffer buffer(GL_PIXEL_PACK_BUFFER, getSizeInBytes());
		glBindBuffer(GL_PIXEL_PACK_BUFFER, buffer.getUid());
		glGetTexImage(GL_TEXTURE_2D, 0, config.format, config.type, NULL);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		#endif
		
		poTexture *tex = new poTexture(width,height,NULL,config);
		glBindTexture(GL_TEXTURE_2D, tex->getUid());
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer.getUid());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, config.format, config.type, NULL);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return tex;
	#endif
}

void poTexture::replace(poImage* image) {
	replace(image->getPixels());
}

void poTexture::replace(const ubyte *pixels) {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->pushTextureState();
	ogl->setTexture(po::TextureState(this));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, config.format, GL_UNSIGNED_BYTE, pixels);
	ogl->popTextureState();
}

bool poTexture::isValid() const {
	return uid > 0;
}

poTextureConfig poTexture::getConfig() const {
	return config;
}

uint poTexture::getUid() const {
	return uid;
}

uint poTexture::getWidth() const {
	return width;
}

uint poTexture::getHeight() const {
	return height;
}

uint poTexture::getChannels() const {
	return channels;
}

uint poTexture::getBitsPerPixel() const {
	return bppForFormatAndType(config.format, config.type);
}

size_t poTexture::getSizeInBytes() const {
	return getWidth() * getHeight() * getBitsPerPixel();
}

poPoint poTexture::getDimensions() const {
	return poPoint(width, height, 0);
}

poRect poTexture::getBounds() const {
	return poRect(0,0,width,height);
}

poColor poTexture::getSourceImagePixel(poPoint p)
{
    if ( sourceImage == NULL )
        return poColor(1,1,1,1);
    else
        return sourceImage->getPixel(p);
}

void poTexture::load(poImage* img) {
	if(!(img && img->isValid())) {
		loadDummyImage();
		return;
	}
	
	load(img, poTextureConfig(formatForChannels(img->getChannels())));
}

void poTexture::load(poImage* img, const poTextureConfig &config) {
	if(!(img && img->isValid())) {
		loadDummyImage();
		return;
	}

	load(img->getWidth(), img->getHeight(), img->getPixels(), config);
}

void poTexture::load(uint width, uint height, int channels, const ubyte *pixels) {
	load(width, height, pixels, poTextureConfig(formatForChannels(channels)));
}

void poTexture::load(uint w, uint h, const ubyte *p, const poTextureConfig &c) {
    
    totalAllocatedTextureMemorySize -= width*height*channels;
    totalAllocatedTextureMemorySize += w*h*channelsForFormat(c.format);
    
	width = w;
	height = h;
	channels = channelsForFormat(c.format);
	config = c;
	
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->pushTextureState();
	
	glGenTextures(1, &uid);
	glBindTexture(GL_TEXTURE_2D, uid);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	
	// set the filters we want
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
	
#ifndef OPENGL_ES
	float trans[] = {0.f, 0.f, 0.f, 0.f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);
#endif
	
	// i'm assuming you're replacing the whole texture anyway
	glTexImage2D(GL_TEXTURE_2D, 
				 0, 
				 config.internalFormat, 
				 width, 
				 height, 
				 0, 
				 config.format, 
				 config.type, 
				 p);
	
	ogl->popTextureState();
}

void poTexture::loadDummyImage() {
	static GLuint dummy = 0;
	if(!dummy) {
		ubyte *pix = new ubyte[20*20*3]();
		for(int y=0; y<20; y++) {
			for(int x=0; x<20; x++) {
				if((y == 0 || x == 0 || y == 19 || x == 19) ||
				   (x == y || x == 19 - y))
				{
					pix[(y*20+x)*3] = 0xFF;
				}
			}
		}
		
		poOpenGLState *ogl = poOpenGLState::get();
		ogl->pushTextureState();
		
		glGenTextures(1, &dummy);
		glBindTexture(GL_TEXTURE_2D, dummy);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        #ifndef OPENGL_ES
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        #else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        #endif
        
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 20, 20, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);
		delete [] pix;
		
		ogl->popTextureState();
	}
	
	uid = dummy;
	width = height = 20;
	channels = 3;
	config = poTextureConfig(GL_RGB).setMinFilter(GL_NEAREST).setMagFilter(GL_NEAREST).setWrapS(GL_TEXTURE_WRAP_S).setWrapT(GL_TEXTURE_WRAP_T);
}

void textureFitExact(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitNone(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitHorizontal(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitVertical(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);

std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	std::vector<poPoint> coords(4);
	std::vector<poPoint> points = rect.getCorners();
	textureFit(rect, tex, fit, align, coords, points);
	return coords;
}

void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	switch(fit) {
		case PO_TEX_FIT_NONE:
			textureFitNone(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_EXACT:
			textureFitExact(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_H:
			textureFitHorizontal(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_V:
			textureFitVertical(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_INSIDE: 
		{
			float new_h = ((float)rect.width) * ((float)tex->getHeight()) / ((float)tex->getWidth());
			if(new_h > rect.height)
				textureFitVertical(rect, tex, align, coords, points);
			else
				textureFitHorizontal(rect, tex, align, coords, points);
			break;
		}
		default:
			;
	}
}

void textureFitExact(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	for(uint i=0; i<points.size(); i++) {
		float s = (points[i].x-rect.x) / rect.width;
		float t = (points[i].y-rect.y) / rect.height;
		coords[i].set(s,1.f-t,0.f);
	}
}


void textureFitNone(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(uint i=0; i<points.size(); i++) {
		float s = (points[i].x-rect.x) / tex->getWidth();
		float t = (points[i].y-rect.y) / tex->getHeight();
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(uint i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
}


void textureFitHorizontal(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float new_w = rect.width;
	float new_h = new_w / (tex->getWidth() / (float)tex->getHeight());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(uint i=0; i<points.size(); i++) {
		float s = (points[i].x-rect.x) / rect.width;
		float t = (points[i].y-rect.y) / new_h;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(uint i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
}

void textureFitVertical(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float new_h = rect.height;
	float new_w = new_h / (tex->getHeight() / (float)tex->getWidth());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(uint i=0; i<points.size(); i++) {
		float s = (points[i].x-rect.x) / new_w;
		float t = (points[i].y-rect.y) / rect.height;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(uint i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
}

