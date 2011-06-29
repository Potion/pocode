//
//  poImage.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poResource.h"
#include "poRect.h"
#include "poEnums.h"
#include "poColor.h"

struct FIBITMAP;

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
	poImage(uint w, uint h, ImageBitDepth bpp, const ubyte *pixels);
	~poImage();
	
	poImage *copy();
	bool isValid() const;
	
	uint width() const;
	uint height() const;
	poPoint dimensions() const;
	ImageBitDepth bpp() const;
	uint pitch() const;
	uint storageSize() const;
	ubyte const*pixels() const;
	
	poColor getPixel(poPoint p) const;
	void setPixel(poPoint p, poColor c);
	void setPixel(poPoint p, poColor c, int stamp_width);
	
	void changeBpp(ImageBitDepth bpp);
	void composite(poImage *img, poRect into);
	void clear();
	
	void flip(poOrientation dir);
	
private:
	void load(const std::string &url);
	void load(const std::string &url, ImageBitDepth bpp);
	void load(uint w, uint h, ImageBitDepth bpp, const ubyte *pix);
	FIBITMAP *bitmap;
};

// will append .cpp to the end of the filename
void writeImageToCHeader(const std::string &filename, poImage *img);
