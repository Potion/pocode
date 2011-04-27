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
	reference = boost::shared_ptr<img_impl>(new img_impl(url));
}

poImage::poImage(const std::string &url, ImageBitDepth bpp) {
	reference = boost::shared_ptr<img_impl>(new img_impl(url, bpp));
}

poImage::poImage(uint w, uint h, ImageBitDepth bpp, ubyte *p) {
	reference = boost::shared_ptr<img_impl>(new img_impl(w,h,bpp,p));
}

poImage::poImage(const poImage &img) {
	reference = img.reference;
}

poImage &poImage::operator=(const poImage &img) {
	reference = img.reference;
	return *this;
}

poImage poImage::copy() {
	poImage img;
	img.reference->bitmap = FreeImage_Clone(reference->bitmap);
	return img;
}

uint poImage::width() const {
	return FreeImage_GetWidth(reference->bitmap);
}

uint poImage::height() const {
	return FreeImage_GetHeight(reference->bitmap);
}

ImageBitDepth poImage::bpp() const {
	return (ImageBitDepth)FreeImage_GetBPP(reference->bitmap);
}

ubyte const*poImage::pixels() const {
	return FreeImage_GetBits(reference->bitmap);
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

poImage::img_impl::img_impl(const std::string &url) {
	bitmap = loadDIB(url);
}

poImage::img_impl::img_impl(const std::string &url, ImageBitDepth bpp) {
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

poImage::img_impl::img_impl(uint w, uint h, ImageBitDepth bpp, ubyte *pix) {
	// i'm not ready to implement this just now
}

poImage::img_impl::~img_impl() {
	FreeImage_Unload(bitmap);
	printf("delete image\n");
}


