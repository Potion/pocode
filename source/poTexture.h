//
//  poTexture.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poPoint.h"
#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poImage.h"
#include "poResource.h"

// This utlity class contains the settings for a poTexture.
class poTextureConfig {
public:
	poTextureConfig();
	poTextureConfig(GLenum format);
	
	poTextureConfig &setFormat(GLenum f)			{format = f; return *this;};
	poTextureConfig &setInternalFormat(GLenum f)	{internalFormat = f; return *this;}
	poTextureConfig &setType(GLenum f)				{type = f; return *this;}
	poTextureConfig &setMinFilter(GLenum f)			{minFilter = f; return *this;}
	poTextureConfig &setMagFilter(GLenum f)			{magFilter = f; return *this;}
	poTextureConfig &setWrapS(GLenum f)				{wrapS = f; return *this;}
	poTextureConfig &setWrapT(GLenum f)				{wrapT = f; return *this;}
	
	GLenum format, internalFormat, type, minFilter, magFilter, wrapS, wrapT;
};


// CLASS NOTES
//
// A poTexture is a type of image that can be attached to poShape2D objects.
//
// It is rare that you will neeed to construct a poTexture directly. Instead, poTexture's
// are usually derived from poImage objects.
//
// poTexture's are also used in frame buffer objects (FBO's) and in video display.
//

class poTexture
{
public:
	// this will make an imageless texture
	// useful for when you just need to allocate space on the graphics card
	poTexture();
	poTexture(const std::string &url);
	poTexture(poImage *img);
	poTexture(poImage *img, const poTextureConfig &config);
	poTexture(uint width, uint height, const ubyte *pixels, const poTextureConfig &config);
	~poTexture();
    
	void				replace(poImage image);
	void				replace(const ubyte *pixels);
	
	bool                isValid() const;
	poTextureConfig		getConfig() const;
	uint				getUid() const;
	uint				getWidth() const;
	uint				getHeight() const;
	uint				getChannels() const;
	poPoint				getDimensions() const;
	poRect				getBounds() const;
	
private:
	void                load(poImage *img);
	void                load(poImage *img, const poTextureConfig &config);
	void				load(uint width, uint height, int channels, const ubyte *pixels);
	void				load(uint width, uint height, const ubyte *pixels, const poTextureConfig &config);
	void				loadDummyImage();
	void                unload();
	
	poTextureConfig     config;
	uint				uid, width, height, channels;
	
	static uint			dummyUid;
};


// figures out tex coords to fit texture in rect
std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align);
// these do the same but make coordinates for each point in points array
// returns texture coordinates thru coords
void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
uint channelsForFormat(GLenum format);

