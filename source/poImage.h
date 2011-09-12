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

struct FIBITMAP;
class poTexture;
class poImageLoader;
class poTextureConfig;

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
	friend class poTexture;
	friend class poImageLoader;
	
public:
	~poImage();
	// you own this one
	poImage *copy();

	operator bool() const;
	
    // IMAGE PROPERTIES
	uint                width() const;
	uint                height() const;
	poPoint             dimensions() const;
	ImageBitDepth       bpp() const;
	uint                pitch() const;
	uint                storageSize() const;
	ubyte const         *pixels() const;
	
    // GET and SET PIXELS
	poColor             getPixel(poPoint p) const;
	void                setPixel(poPoint p, poColor c);
	void                setPixel(poPoint p, poColor c, int stamp_width);
	
    // IMAGE OPERATIONS
	void                changeBpp(ImageBitDepth bpp);
	void                composite(poImage *img, poRect into);
	void                blur(int kernel_size, float sig);
	void                clear();
	void                flip(poOrientation dir);

    // IMAGE RESIZING
	void                resizeMaxDimension(float max_dim);
	void                resizeMinDimension(float min_dim);
	void                resizeWidth(float w);
	void                resizeHeight(float h);
	void                resize(float w, float h);
	
    // TEXTURE FROM IMAGE
	poTexture*          texture();
	poTexture*          texture(poTextureConfig config);
	
	// IMAGE URL
	std::string         url() const;
	
private:
	poImage();
	poImage(const std::string &url);
	poImage(const std::string &url, ImageBitDepth bpp);
	poImage(uint w, uint h, ImageBitDepth bpp, const ubyte *pixels);
	poImage(const poImage &img);

	void                load(const std::string &url);
	void                load(const std::string &url, ImageBitDepth bpp);
	void                load(uint w, uint h, ImageBitDepth bpp, const ubyte *pix);
	
	FIBITMAP            *bitmap;
	poTexture           *tex;
	std::string         _url;
};

// will append .cpp to the end of the filename
void writeImageToCHeader(const std::string &filename, poImage *img);




