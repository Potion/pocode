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
#include "poTextureConfig.h"

class poImage;

/* usually you'll create a texture thru an image
 * only in some cases will you want to create a texture without an image
 * for example: fbo or video where the entire imaging process happens on the graphics card
 */
class poTexture 
{
	friend class poImage;

public:
	// this will make an imageless texture
	// useful for when you just need to allocate space on the graphics card
	poTexture(uint width, uint height, poTextureConfig config);
	~poTexture();
	
	bool opaqueAtPoint(poPoint p) const;
	poColor colorAtPoint(poPoint p) const;

	uint width() const;
	uint height() const;
	
	void bind(uint unit=0) const;
	void unbind(uint unit=0) const;

	void load();
	void unload();
	bool isLoaded() const;

	GLuint uid;
	poTextureConfig config;
	float s, t;

private:
	void loadDummy();
	
	poTexture();
	poTexture(poImage *img);
	// this isn't too thurough right now
	// you can screw yourself if you demand a format incompatible with the image
	poTexture(poImage *img, poTextureConfig config);
	poImage *image;
	
	// will only be used when the image is null
	uint _width, _height;
};
