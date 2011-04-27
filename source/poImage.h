//
//  poImage.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include <FreeImage.h>

enum ImageBitDepth {
	IMAGE_8 = 8,
	IMAGE_16 = 16,
	IMAGE_24 = 24,
	IMAGE_32 = 32
};

class poImage {
public:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, ImageBitDepth bpp);
	poImage(uint w, uint h, ImageBitDepth bpp, ubyte *pixels);
	poImage(const poImage &img);
	poImage &operator=(const poImage &img);
	poImage copy();
	
	uint width() const;
	uint height() const;
	ImageBitDepth bpp() const;
	ubyte const*pixels() const;
	
private:
	struct img_impl {
		~img_impl();
		img_impl(const std::string &url);
		img_impl(const std::string &url, ImageBitDepth bpp);
		img_impl(uint w, uint h, ImageBitDepth bpp, ubyte *pix);

		FIBITMAP *bitmap;
	};
	boost::shared_ptr<img_impl> reference;
};


