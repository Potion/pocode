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
#include "poResource.h"

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

class poImage
{
friend std::ostream &operator<<(std::ostream &out, const poImage *img);

public:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, uint numChannels);
	poImage(uint w, uint h, uint numChannels, const ubyte *pixels);
	~poImage();

	poImage*			copy();
	
    // IMAGE PROPERTIES
	bool				isValid() const;
	uint                getWidth() const;
	uint                getHeight() const;
	uint				getChannels() const;
	poPoint             getDimensions() const;
	uint                getPitch() const;
	uint                getStorageSize() const;
	
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
	void                clear();

    // IMAGE RESIZING
	void                resizeMaxDimension(float max_dim);
	void                resizeMinDimension(float min_dim);
	void                resizeWidth(float w);
	void                resizeHeight(float h);
	void                resize(float w, float h);
	
	// IMAGE URL, COULD BE ""
	std::string         getUrl() const;
	
private:
	void                load(const std::string &url);
	void                load(const std::string &url, uint c);
	void                load(uint w, uint h, uint c, const ubyte *pix);
	
	FIBITMAP            *bitmap;
	std::string         url;
};




