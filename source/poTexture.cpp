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
		case GL_LUMINANCE:
		case GL_ALPHA:
			return 1;
		case GL_LUMINANCE_ALPHA:
			return 2;
		case GL_RGB:
			return 3;
		case GL_RGBA:
			return 4;
	}
	return 1;
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

poTexture::TextureImpl::TextureImpl()
:	uid(0), width(0), height(0), channels(0), config()
{}

poTexture::TextureImpl::TextureImpl(uint w, uint h, const ubyte *p, const poTextureConfig &c) {
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
}

poTexture::TextureImpl::~TextureImpl() {
	if(uid > 0) {
		glDeleteTextures(1, &uid);
		uid = 0;
	}
}

poTexture::poTexture() {}

poTexture::poTexture(const std::string &url) {
	load(poImage(url));
}

poTexture::poTexture(poImage img) {
	load(img);
}

poTexture::poTexture(poImage img, const poTextureConfig &config) {
	load(img, config);
}

poTexture::poTexture(uint width, uint height, const ubyte *pixels, const poTextureConfig &config) {
	load(width, height, pixels, config);
}

void poTexture::replace(poImage image) {
	replace(image.getPixels());
}

void poTexture::replace(const ubyte *pixels) {
	poOpenGLState *ogl = poOpenGLState::get();
	ogl->pushTextureState();
	ogl->setTexture(po::TextureState(*this));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, shared->width, shared->height, shared->config.format, GL_UNSIGNED_BYTE, pixels);
	ogl->popTextureState();
}

bool poTexture::isValid() const {
	return shared && shared->uid > 0;
}

poTextureConfig poTexture::getConfig() const {
	return shared->config;
}

uint poTexture::getUid() const {
	return shared->uid;
}

uint poTexture::getWidth() const {
	return shared->width;
}

uint poTexture::getHeight() const {
	return shared->height;
}

uint poTexture::getChannels() const {
	return shared->channels;
}

poPoint poTexture::getDimensions() const {
	return poPoint(shared->width, shared->height, 0);
}

poRect poTexture::getBounds() const {
	return poRect(0,0,shared->width,shared->height);
}

void poTexture::load(poImage img) {
	if(!img.isValid()) {
		loadDummyImage();
		return;
	}
	
	load(img, poTextureConfig(formatForChannels(img.getChannels())));
}

void poTexture::load(poImage img, const poTextureConfig &config) {
	if(!img.isValid()) {
		loadDummyImage();
		return;
	}

	load(img.getWidth(), img.getHeight(), img.getPixels(), config);
}

void poTexture::load(uint width, uint height, int channels, const ubyte *pixels) {
	load(width, height, pixels, poTextureConfig(formatForChannels(channels)));
}

void poTexture::load(uint width, uint height, const ubyte *pixels, const poTextureConfig &config) {
	shared.reset(new TextureImpl(width,height,pixels,config));
}

void poTexture::loadDummyImage() {
	static boost::shared_ptr<TextureImpl> dummy;
	if(!dummy) {
		dummy.reset(new TextureImpl);
		
		ubyte *pix = new ubyte[20*20*3]();
		for(int y=0; y<20; y++) 
			for(int x=0; x<20; x++) {
				if((y == 0 || x == 0 || y == 19 || x == 19) ||
				   (x == y || x == 19 - y))
				{
					pix[(y*20+x)*3] = 0xFF;
				}
			}
		
		dummy.reset(new TextureImpl(20,20,pix,poTextureConfig(GL_RGB)));
	}
	shared = dummy;
}

void textureFitExact(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitNone(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitHorizontal(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
void textureFitVertical(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);

std::vector<poPoint> textureFit(poRect rect, poTexture tex, poTextureFitOption fit, poAlignment align) {
	std::vector<poPoint> coords(4);
	std::vector<poPoint> points = rect.getCorners();
	textureFit(rect, tex, fit, align, coords, points);
	return coords;
}

void textureFit(poRect rect, poTexture tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
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
			float new_h = rect.width * (tex.getHeight() / (float)tex.getHeight());
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

void textureFitExact(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
		
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width - xoff;
		float t = points[i].y / rect.height - yoff;
		coords[i].set(s,1.f-t,0.f);
	}
}


void textureFitNone(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / tex.getWidth() - xoff;
		float t = points[i].y / tex.getHeight() - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
}


void textureFitHorizontal(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	float new_w = rect.width;
	float new_h = new_w / (tex.getWidth() / (float)tex.getHeight());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width - xoff;
		float t = points[i].y / new_h - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
}

void textureFitVertical(poRect rect, poTexture tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	float new_h = rect.height;
	float new_w = new_h / (tex.getHeight() / (float)tex.getWidth());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / new_w - xoff;
		float t = points[i].y / rect.height - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i].y = max.y - coords[i].y;
		coords[i] -= offset;
	}
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




