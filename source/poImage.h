//
//  poImage.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poRect.h"
#include "poEnums.h"
#include "poColor.h"
#include "poTextureConfig.h"

struct FIBITMAP;
class poTexture;
class poImageLoader;

class poImage 
{
	friend class poTexture;
	friend class poImageLoader;
	
public:
	~poImage();
	// you own this one
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
	void blur(int kernel_size, float sig);
	void clear();
	
	void flip(poOrientation dir);

	void resizeMaxDimension(float max_dim);
	void resizeMinDimension(float min_dim);
	void resizeWidth(float w);
	void resizeHeight(float h);
	void resize(float w, float h);
	
	poTexture *texture();
	poTexture *texture(poTextureConfig config);
	
private:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, ImageBitDepth bpp);
	poImage(uint w, uint h, ImageBitDepth bpp, const ubyte *pixels);
	poImage(const poImage &img);

	void load(const std::string &url);
	void load(const std::string &url, ImageBitDepth bpp);
	void load(uint w, uint h, ImageBitDepth bpp, const ubyte *pix);
	
	FIBITMAP *bitmap;
	poTexture *tex;
};

// will append .cpp to the end of the filename
void writeImageToCHeader(const std::string &filename, poImage *img);




