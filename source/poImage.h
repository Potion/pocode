//
//  poImage.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include <FreeImage.h>
#include "poResource.h"

enum ImageBitDepth {
	IMAGE_8 = 8,
	IMAGE_16 = 16,
	IMAGE_24 = 24,
	IMAGE_32 = 32
};

class poImage 
	: public poResource
{
public:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, ImageBitDepth bpp);
	poImage(uint w, uint h, ImageBitDepth bpp, ubyte *pixels);
	~poImage();
	
	poImage *copy();
	
	uint width() const;
	uint height() const;
	ImageBitDepth bpp() const;
	uint pitch() const;
	uint storageSize() const;
	ubyte const*pixels() const;
	
private:
	void load(const std::string &url);
	void load(const std::string &url, ImageBitDepth bpp);
	void load(uint w, uint h, ImageBitDepth bpp, ubyte *pix);
	FIBITMAP *bitmap;
};
