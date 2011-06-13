//
//  poImage.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poImage.h"
#include <FreeImage.h>

static void loadFreeImageIfNeeded() {
	static bool free_image_loaded = false;
	if(!free_image_loaded) {
		FreeImage_Initialise();
		free_image_loaded = true;
	}
}

poImage::poImage() {loadFreeImageIfNeeded();}

poImage::poImage(const std::string &url) {
	loadFreeImageIfNeeded();
	load(url);
}

poImage::poImage(const std::string &url, ImageBitDepth bpp) {
	loadFreeImageIfNeeded();
	load(url, bpp);
}

poImage::poImage(uint w, uint h, ImageBitDepth bpp, ubyte *p) {
	loadFreeImageIfNeeded();
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

void poImage::changeBpp(ImageBitDepth bpp) {
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

void poImage::composite(poImage *img, poRect into) {
	FreeImage_Paste(bitmap, img->bitmap, into.origin.x, into.origin.y, 256);
}

void poImage::flip(poOrientation dir) {
	if(dir == PO_VERTICAL)
		FreeImage_FlipVertical(bitmap);
	else
		FreeImage_FlipHorizontal(bitmap);
}

FIBITMAP *loadDIB(const std::string &url) {
	// figure out if the file exists
	
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
	changeBpp(bpp);
}

void poImage::load(uint w, uint h, ImageBitDepth bpp, ubyte *pix) {
	if(pix != NULL)
		bitmap = FreeImage_ConvertFromRawBits(pix, w, h, w, bpp, 0, 0, 0);
	else
		bitmap = FreeImage_Allocate(w, h, bpp);
}


