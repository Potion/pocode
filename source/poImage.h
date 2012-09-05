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
//  poImage.h
//  pocode
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poRect.h"
#include "poEnums.h"
#include "poColor.h"
#include "poThreadCenter.h"

#include <string>
#include <ostream>

struct FIBITMAP;

// CLASS NOTES
//
// poImage is used to load and edit images. It can also be used to create images from scratch.
//
// You cannot construct a new poImage directly. You can load a new image as follows:
//
//      poImage* myImage = getImage("images/sample.jpg");
//
// Using getImage() ensures that the same image is not loaded and constructed multiple times.
//
// poImage has no built-in methods for drawing. Instead, construct a poRectShape as follows:
//
//      poRectShape* S = new poRectShape("images/sample.jpg");
//      addChild( S );
//
// This will load a poImage, generate an OpenGL texture from the image, and attach the
// texture to the newly created poRectShape. By default, the poRectShape will be the
// native size of the image.
//

class poImage {
	friend std::ostream &operator<<(std::ostream &out, const poImage &img);

public:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, uint numChannels);
	poImage(uint w, uint h, uint numChannels, const ubyte *pixels);
	~poImage();

	poImage*			copy();
	void				save(const std::string &loc);
	
    // IMAGE PROPERTIES
	bool				isValid() const;
	bool				hasAlpha() const;
	uint                getWidth() const;
	uint                getHeight() const;
	uint				getChannels() const;
	poPoint             getDimensions() const;
	uint                getPitch() const;
	uint                getStorageSize() const;
    bool                isScaled();
	
    // GET and SET PIXELS
	ubyte const*		getPixels() const;
	poColor             getPixel(poPoint p) const;
	void                setPixel(poPoint p, poColor c);
	void                setPixel(poPoint p, poColor c, int stamp_width);
	
    // IMAGE OPERATIONS
	void                setNumChannels(uint numChannels);
	void                composite(poImage *img, poPoint into, float blend);
	void                blur(int kernel_size, float sigma, int stepMultiplier=1);
	void                flip(poOrientation dir);
	void				fill(poColor c);
    void                invert();
	void                clear();

    // IMAGE RESIZING
	void                resizeMaxDimension(float max_dim);
	void                resizeMinDimension(float min_dim);
	void                resizeWidth(float w);
	void                resizeHeight(float h);
	void                resize(float w, float h);
	
	// IMAGE URL, COULD BE "" IF NOTHING LOADED
	std::string         getUrl() const;
    
    static int          getTotalAllocatedImageMemorySize() { return totalAllocatedImageMemorySize; };
    
private:
	void                load(const std::string &url);
	void                load(const std::string &url, uint numChannels);
	void                load(uint w, uint h, uint c, const ubyte *pix);
	
	FIBITMAP            *bitmap;
    bool                scaledBitmapFound;
    void                setUrl(const std::string url);
	std::string         url;
    
    static int          totalAllocatedImageMemorySize;
};


static const std::string ImageLoadingCompleteMessage    = "IMAGE_LOADING_COMPLETE_MESSAGE";
static const std::string ImageLoadSuccessMessage        = "IMAGE_LOAD_SUCCESS_MESSAGE";
static const std::string ImageLoadFailureMessage        = "IMAGE_LOAD_FAILURE_MESSAGE";

class poImageLoaderWorker : public poWorker {
public:
	poImageLoaderWorker(std::string url);
	virtual ~poImageLoaderWorker();
	
	void workerFunc();
private:
    std::string url;
};


