/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  poImage.cpp
//  pocode
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poImage.h"
#include "poHelpers.h"
#include <FreeImage.h>

static void loadFreeImageIfNeeded() {
	static bool free_image_loaded = false;
	if(!free_image_loaded) {
		FreeImage_Initialise();
		free_image_loaded = true;
	}
}

int poImage::totalAllocatedImageMemorySize = 0;


poImage::poImage()
:	bitmap(NULL)
,	url("")
{}

poImage::poImage(const std::string &url)
:	bitmap(NULL)
,	url("")
{
	load(url);
}

poImage::poImage(const std::string &url, uint c) 
:	bitmap(NULL)
,	url("")
{
	load(url, c);
}

poImage::poImage(uint w, uint h, uint c, const ubyte *p) 
:	bitmap(NULL)
,	url("")
{
	load(w, h, c, p);
}

poImage::~poImage() {
    totalAllocatedImageMemorySize -= FreeImage_GetDIBSize(bitmap);
	FreeImage_Unload(bitmap);
	bitmap = NULL;
}

poImage* poImage::copy() {
	poImage* image = new poImage;
	if(isValid()) {
		image->bitmap = FreeImage_Clone(bitmap);
		image->url = url;
	}
	return image;
}

void poImage::save(const std::string &loc) {
	if(isValid()) {
		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(loc.c_str());
		FreeImage_Save(fif, bitmap, loc.c_str());
	}
}

bool poImage::isValid() const {
	return bitmap != NULL;
}

bool poImage::hasAlpha() const {
	return FreeImage_GetColorType(bitmap) == FIC_RGBALPHA;
}

uint poImage::getWidth() const {
	return FreeImage_GetWidth(bitmap);
}

uint poImage::getHeight() const {
	return FreeImage_GetHeight(bitmap);
}

uint poImage::getChannels() const {
	return FreeImage_GetBPP(bitmap) / 8;
}

poPoint poImage::getDimensions() const {
	return poPoint(getWidth(), getHeight());
}

uint poImage::getPitch() const {
	return FreeImage_GetPitch(bitmap);
}

uint poImage::getStorageSize() const {
	return getPitch() * getHeight();
}

ubyte const*poImage::getPixels() const {
	return FreeImage_GetBits(bitmap);
}

poColor poImage::getPixel(poPoint p) const {
        
	if(!isValid() || p.x < 0 || p.y < 0 || p.x >= getWidth() || p.y >=getHeight())
		return poColor();

	uint x = p.x;
	uint y = p.y;
	
	BYTE* bits = FreeImage_GetScanLine(bitmap, y);
	poColor ret;
	
	switch(getChannels()) {
		case 1:
			ret.set255(bits[x], bits[x], bits[x], 255);
			break;
		case 2:
			ret.set255(bits[x*2], bits[x*2], bits[x*2], bits[x*2+1]);
			break;
		case 3:
			ret.set255(bits[x*3+0], bits[x*3+1], bits[x*3+2], 255);
			break;
		case 4:
			ret.set255(bits[x*4+0], bits[x*4+1], bits[x*4+2], bits[x*4+3]);
			break;
		default:
			;
	}
	
	return ret;
}

void poImage::setPixel(poPoint p, poColor c) {
	if(p.x < 0 || p.y < 0 || p.x >= getWidth() || p.y >=getHeight())
		return;
	
	uint x = p.x;
	uint y = p.y;
	
	BYTE *bits = FreeImage_GetScanLine(bitmap, y);
	switch(getChannels()) {
		case 1:
			bits[x] = (((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * c.A) * 255;
			break;
		case 2:
			bits[x*2] = ((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * 255;
			bits[x*2+1] = c.A * 255;
			break;
		case 3:
			bits[x*3] = c.R * c.A * 255;
			bits[x*3+1] = c.G * c.A * 255;
			bits[x*3+2] = c.B * c.A * 255;
			break;
		case 4:
			bits[x*4] = c.R * 255;
			bits[x*4+1] = c.G * 255;
			bits[x*4+2] = c.B * 255;
			bits[x*4+3] = c.A * 255;
			break;
	}
}

void poImage::setPixel(poPoint p, poColor c, int stamp_width) {
	for(int y=-stamp_width/2; y<stamp_width/2; y++) {
		for(int x=-stamp_width/2; x<stamp_width/2; x++) {
			setPixel(p + poPoint(x,y), c);
		}
	}
}

void poImage::setNumChannels(uint c) {
	if(isValid() && FreeImage_GetBPP(bitmap) != c*8) {
		FIBITMAP *dst = NULL;
		switch(c) {
			case 1:	
				dst = FreeImage_ConvertTo8Bits(bitmap); 
				break;
			case 2:	
				dst = FreeImage_ConvertTo16Bits565(bitmap); 
				break;
			case 3:	
				dst = FreeImage_ConvertTo24Bits(bitmap); 
				break;
			case 4:	
				dst = FreeImage_ConvertTo32Bits(bitmap); 
				break;
		}
		FreeImage_Unload(bitmap);
		bitmap = dst;
	}
}

void poImage::composite(poImage *img, poPoint into, float blend) {
	FreeImage_Paste(bitmap, img->bitmap, into.x, into.y, blend*256);
}

poColor operator*(const poColor& c, float f) {
	poColor response(c);
	response.R *= f;
	response.G *= f;
	response.B *= f;
	response.A *= f;
	return response;
}

poColor operator/(const poColor& c, float f) {
	poColor response(c);
	response.R /= f;
	response.G /= f;
	response.B /= f;
	response.A /= f;
	return response;
}

poColor operator+(const poColor &a, const poColor &b) {
	poColor response(a);
	response.R = a.R + b.R;
	response.G = a.G + b.G;
	response.B = a.B + b.B;
	response.A = a.A + b.A;
	return response;
}

void poImage::blur(int kernel_size, float sig, int stepMultiplier) {
	float e = 2.718281828f;
	int hk = kernel_size / 2;
	
	float *kernel = new float[kernel_size];
	for(int i=0; i<kernel_size; i++) {
		float contrib = powf(i - hk, 2) / (2 * sig*sig);
		kernel[i] = powf(e, -contrib);
	}
	
	poImage* tmp = copy();
	int width = getWidth();
	int height = getHeight();
	
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			poColor sum;
			float k_tot = 0;
			
			for(int i=-hk; i<hk; i++) {
				int stp = i * stepMultiplier;
				if(x+stp < 0 || x+stp >= width)
					continue;
				
				float k = kernel[i+hk];
				k_tot += k;
				
				poColor c = getPixel(poPoint(x+stp,y));
				
				sum = sum + (c*k);
			}
			
			tmp->setPixel(poPoint(x,y), sum/k_tot);
		}
	}
	
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			poColor sum;
			float k_tot = 0;
			
			for(int i=-hk; i<hk; i++) {
				int stp = i * stepMultiplier;
				if(y+stp < 0 || y+stp >= height)
					continue;
				
				float k = kernel[i+hk];
				k_tot += k;
				
				poColor c = tmp->getPixel(poPoint(x,y+stp));
				
				sum = sum + (c*k);
			}
			
			setPixel(poPoint(x,y), sum/k_tot);
		}
	}
	
	delete tmp;
	delete [] kernel;
}

void poImage::flip(poOrientation dir) {
	if(dir == PO_VERTICAL)
		FreeImage_FlipVertical(bitmap);
	else
		FreeImage_FlipHorizontal(bitmap);
}

void poImage::fill(poColor c) {
	unsigned char color[4] = {c.R*255, c.G*255, c.B*255, c.A*255};
	FreeImage_FillBackground(bitmap, color);
}

void poImage::resizeMaxDimension(float max_dim) {
	int newW = getWidth();
	int newH = getHeight();
	
	if(newW > newH)
		resizeWidth(max_dim);
	else
		resizeHeight(max_dim);
}

void poImage::resizeMinDimension(float min_dim) {
	int newW = getWidth();
	int newH = getHeight();
	
	if(newW < newH)
		resizeWidth(min_dim);
	else
		resizeHeight(min_dim);
}

void poImage::resizeWidth(float w) {
	float aspect = getHeight() / (float)getWidth();
	resize(w, w * aspect);
}

void poImage::resizeHeight(float h) {
	float aspect = getWidth() / (float)getHeight();
	resize(h * aspect, h);
}

void poImage::resize(float w, float h) {
	FIBITMAP *img = FreeImage_Rescale(bitmap, w, h, FILTER_CATMULLROM);

	FreeImage_Unload(bitmap);
	bitmap = img;
}

std::string poImage::getUrl() const {
	return url;
}

void poImage::clear() {
	ubyte color[] = {0,0,0,0};
	FreeImage_FillBackground(bitmap, &color[0]);
}

FIBITMAP *loadDIB(const std::string &url) {
	// figure out if the file exists
	if(!fs::exists(url)) {
		printf("poImage: image file not found (%s)\n", url.c_str());
		return NULL;
	}
	
	loadFreeImageIfNeeded();

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(url.c_str());
//	if(fif == FIF_UNKNOWN)
//		fif = FreeImage_GetFIFFromFilename(url.c_str());
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

	unsigned bpp = FreeImage_GetBPP(dib);
	if(bpp == 24 || bpp == 32) {
		// there has got to be a more efficient way of doing this
		FIBITMAP *red = FreeImage_GetChannel(dib,FICC_RED);
		FIBITMAP *blue = FreeImage_GetChannel(dib,FICC_BLUE);
		
		FreeImage_SetChannel(dib,red,FICC_BLUE);
		FreeImage_SetChannel(dib,blue,FICC_RED);
		
		FreeImage_Unload(red);
		FreeImage_Unload(blue);
	}
	
	return dib;
}

void poImage::load(const std::string &url) {
	FIBITMAP *bmp = loadDIB(url);
	if(bmp) {
		bitmap = bmp;
		this->url = url;
        totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
	}
}

void poImage::load(const std::string &url, uint c) {
	FIBITMAP *bmp = loadDIB(url);
	if(bmp) {
		bitmap = bmp;
		this->url = url;
		setNumChannels(c);
        totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
	}
}

void poImage::load(uint w, uint h, uint c, const ubyte *pix) {
	if(bitmap)
    {
        totalAllocatedImageMemorySize -= FreeImage_GetDIBSize(bitmap);
		FreeImage_Unload(bitmap);
    }
	
	if(pix != NULL)
		bitmap = FreeImage_ConvertFromRawBits(const_cast<ubyte*>(pix), w, h, w*c, c*8, 0,0,0);
	else {
		bitmap = FreeImage_Allocate(w, h, c*8);
		char black[] = {0,0,0,0};
		FreeImage_FillBackground(bitmap, black);
	}
    
    totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
}

std::ostream &operator<<(std::ostream &out, const poImage *img) {
	return out << "image('" << img->getUrl() << "')";
}

//void writeImageToCHeader(const std::string &str, poImage *img) {
//	std::string filename = str + "Image.cpp";
//	FILE *f = fopen(filename.c_str(), "w+");
//	fprintf(f,
//			"\n#include \"%sImage.h\"\n\n"
//			"static const uint width = %d;\n"
//			"static const uint height = %d;\n"
//			"static const uint format = %d;\n"
//			"static const ubyte bytes[] = {\n",
//			str.c_str(), img->width(), img->height(), (int)img->bpp());
//	for(int h=0; h<img->height(); h++) {
//		fprintf(f,"\t");
//		for(int w=0; w<img->width(); w++) {
//			poColor color = img->getPixel(poPoint(w,h));
//			fprintf(f,"0x%X,0x%X,0x%X,", uint(color.R*255), uint(color.G*255), uint(color.B*255));
//		}
//		if(h == img->height()-1)
//			fseek(f, -1, SEEK_CUR);
//		fprintf(f,"\n");
//	}
//	fprintf(f,
//			"};\n"
//			"poImage *get%sImage() {\n"
//			"	static poImage *img = NULL;\n"
//			"	if(!img) {\n"
//			"		img = new poImage(width,height,(ImageBitDepth)format,&bytes[0]);\n"
//			"	}\n"
//			"	return img->copy();\n"
//			"}\n\n",
//			str.c_str());
//	fclose(f);
//	
//	filename = str + "Image.h";
//	f = fopen(filename.c_str(), "w");
//	fprintf(f,
//			"\n#pragma once\n\n"
//			"#include \"poImage.h\"\n\n"
//			"poImage *get%sImage();\n",
//			str.c_str());
//	fclose(f);
//}
