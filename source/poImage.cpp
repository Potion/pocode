//
//  poImage.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poImage.h"

poImage::poImage() {}

poImage::poImage(const std::string &url) {
	load(url);
}

poImage::poImage(const std::string &url, ImageBitDepth bpp) {
	load(url, bpp);
}

poImage::poImage(uint w, uint h, ImageBitDepth bpp, ubyte *p) {
	load(w, h, bpp, p);
}

poImage::~poImage() {
	FreeImage_Unload(bitmap);
}

poImage *poImage::copy() {
	poImage *img = new poImage();
	img->bitmap = FreeImage_Clone(bitmap);
	return img;
}

uint poImage::width() const {
	return FreeImage_GetWidth(bitmap);
}

uint poImage::height() const {
	return FreeImage_GetHeight(bitmap);
}

ImageBitDepth poImage::bpp() const {
	return (ImageBitDepth)FreeImage_GetBPP(bitmap);
}

uint poImage::pitch() const {
	return FreeImage_GetPitch(bitmap);
}

uint poImage::storageSize() const {
	return pitch() * height();
}

ubyte const*poImage::pixels() const {
	return FreeImage_GetBits(bitmap);
}

FIBITMAP *loadDIB(const std::string &url) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(url.c_str());
	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(url.c_str());
	if(fif == FIF_UNKNOWN)
		return NULL;
	
	FIBITMAP *dib = NULL;
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, url.c_str());
	if(!dib)
		return NULL;
	
	FreeImage_FlipVertical(dib);
	return dib;
}

void poImage::load(const std::string &url) {
	bitmap = loadDIB(url);
}

void poImage::load(const std::string &url, ImageBitDepth bpp) {
	bitmap = loadDIB(url);
	if(bitmap && FreeImage_GetBPP(bitmap) != bpp) {
		FIBITMAP *dst = NULL;
		
		switch(bpp) {
			case IMAGE_8:	
				dst = FreeImage_ConvertTo8Bits(bitmap); 
				break;
			case IMAGE_16:	
				dst = FreeImage_ConvertTo16Bits565(bitmap); 
				break;
			case IMAGE_24:	
				dst = FreeImage_ConvertTo24Bits(bitmap); 
				break;
			case IMAGE_32:	
				dst = FreeImage_ConvertTo32Bits(bitmap); 
				break;
		}
		
		FreeImage_Unload(bitmap);
		bitmap = dst;
	}
}

void poImage::load(uint w, uint h, ImageBitDepth bpp, ubyte *pix) {
	// i'm not ready to implement this just now
}


