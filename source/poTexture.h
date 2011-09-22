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

class poImage;
class poTextureConfig;

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
	friend class    poImage;

public:
	// this will make an imageless texture
	// useful for when you just need to allocate space on the graphics card
	poTexture(uint width, uint height, poTextureConfig config);
	~poTexture();
    
    // TEXTURE PROPERTIES
    uint                width() const;
	uint                height() const;
	
    // PIXEL LOOKUP
    // These methods refer to the poImage used to generate the texture.
    // opaqueAtPoint() is used by pointInside() in poShape2D objects
    // to limit hit testing to just the opaque part of an image. 
	bool                opaqueAtPoint(poPoint p) const;
	poColor             colorAtPoint(poPoint p) const;

    // TEXTURE LOADING
    // Textures must be loaded onto the graphics card before they are used.
    // Textures may be removed from the graphcis card using the unload() method.
    // In general, textures are loaded once and not unloaded until the program exits. 
	void                load();
	void                unload();
	bool                isLoaded() const;

    // TEXTURE CONFIGURATION
	poTextureConfig     config;
    GLuint              uid;
	float               s, t;
	
	poImage const*      image() const;

private:
	void                loadDummy();
	
	poTexture();
	poTexture(poImage *img);
	// this isn't too thorough right now
	// you can screw yourself if you demand a format incompatible with the image
	poTexture(poImage *img, poTextureConfig config);
    
	poImage             *_image;
	
	// will only be used when the image is null
	uint                _width, _height;
};


// figures out tex coords to fit texture in rect
std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align);
// these do the same but make coordinates for each point in points array
// returns texture coordinates thru coords
void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);


