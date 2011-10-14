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

class poTexture : public poResource
{
public:
	// this will make an imageless texture
	// useful for when you just need to allocate space on the graphics card
	poTexture();
	poTexture(poImage *img);
	poTexture(poImage *img, const poTextureConfig &config);
	~poTexture();
    
    // TEXTURE LOADING
    // Textures must be loaded onto the graphics card before they are used.
    // Textures may be removed from the graphcis card using the unload() method.
	void                load(poImage *img);
	void                load(poImage *img, const poTextureConfig &config);
	void				load(uint width, uint height, int channels, const ubyte *pixels);
	// figures out the channel count from the requested format
	void				load(uint width, uint height, const ubyte *pixels, const poTextureConfig &config);
	void                unload();
	bool                isLoaded() const;

    // TEXTURE CONFIGURATION
	// treat these as read-only
    GLuint              uid;
	float               s, t;
	poTextureConfig     config;
	uint				width, height, channels;
};


// figures out tex coords to fit texture in rect
std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align);
// these do the same but make coordinates for each point in points array
// returns texture coordinates thru coords
void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
uint channelsForFormat(GLenum format);

