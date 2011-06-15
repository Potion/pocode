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

poImage::poImage(const std::string &url)
:	valid(false)
{
	loadFreeImageIfNeeded();
	load(url);
}

poImage::poImage(const std::string &url, ImageBitDepth bpp) 
:	valid(false)
{
	loadFreeImageIfNeeded();
	load(url, bpp);
}

poImage::poImage(uint w, uint h, ImageBitDepth bpp, const ubyte *p) 
:	valid(false)
{
	loadFreeImageIfNeeded();
	load(w, h, bpp, p);
}

poImage::~poImage() {
	FreeImage_Unload(bitmap);
}

poImage *poImage::copy() {
	poImage *img = new poImage();
	img->bitmap = FreeImage_Clone(bitmap);
	img->valid = valid;
	return img;
}

bool poImage::isValid() const {
	return valid;
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

poColor poImage::getPixel(uint x, uint y) const {
	BYTE* bits = FreeImage_GetScanLine(bitmap, y);
	poColor ret;
	
	switch(bpp()) {
		case IMAGE_8:
			ret.set255(bits[x], bits[x], bits[x], 255);
			break;
		case IMAGE_16:
			ret.set255(bits[x*2], bits[x*2], bits[x*2], bits[x*2+1]);
			break;
		case IMAGE_24:
			ret.set255(bits[x*3], bits[x*3+1], bits[x*3+2], 255);
			break;
		case IMAGE_32:
			ret.set255(bits[x*4], bits[x*4+1], bits[x*4+2], bits[x*4+3]);
			break;
	}
	
	return ret;
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
	if(fif == FIF_UNKNOWN) {
		printf("poImage: image isn't a supported file type (%s)\n", url.c_str());
		return NULL;
	}
	
	FIBITMAP *dib = NULL;
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, url.c_str());
	if(!dib) {
		printf("poImage: image file not found (%s)\n", url.c_str());
		return NULL;
	}
	
	FreeImage_FlipVertical(dib);
	return dib;
}

void poImage::load(const std::string &url) {
	bitmap = loadDIB(url);
	valid = bitmap != NULL;
}

void poImage::load(const std::string &url, ImageBitDepth bpp) {
	bitmap = loadDIB(url);
	valid = bitmap != NULL;
	changeBpp(bpp);
}

void poImage::load(uint w, uint h, ImageBitDepth bpp, const ubyte *pix) {
	if(pix != NULL)
		bitmap = FreeImage_ConvertFromRawBits(const_cast<ubyte*>(pix), w, h, w*(bpp/8), bpp, 0,0,0);
	else
		bitmap = FreeImage_Allocate(w, h, bpp);
}

void writeImageToCHeader(const std::string &str, poImage *img) {
	std::string filename = str + "Image.cpp";
	FILE *f = fopen(filename.c_str(), "w+");
	fprintf(f,
			"\n#include \"%sImage.h\"\n\n"
			"static const uint width = %d;\n"
			"static const uint height = %d;\n"
			"static const uint format = %d;\n"
			"static const ubyte bytes[] = {\n",
			str.c_str(), img->width(), img->height(), (int)img->bpp());
	for(int h=0; h<img->height(); h++) {
		fprintf(f,"\t");
		for(int w=0; w<img->width(); w++) {
			poColor color = img->getPixel(w,h);
			fprintf(f,"0x%X,0x%X,0x%X,", uint(color.R*255), uint(color.G*255), uint(color.B*255));
		}
		if(h == img->height()-1)
			fseek(f, -1, SEEK_CUR);
		fprintf(f,"\n");
	}
	fprintf(f,
			"};\n"
			"poImage *get%sImage() {\n"
			"	static poImage *img = NULL;\n"
			"	if(!img) {\n"
			"		img = new poImage(width,height,(ImageBitDepth)format,&bytes[0]);\n"
			"	}\n"
			"	return img->copy();\n"
			"}\n\n",
			str.c_str());
	fclose(f);
	
	filename = str + "Image.h";
	f = fopen(filename.c_str(), "w");
	fprintf(f,
			"\n#pragma once\n\n"
			"#include \"poImage.h\"\n\n"
			"poImage *get%sImage();\n",
			str.c_str());
	fclose(f);
}

