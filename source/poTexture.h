//
//  poTexture.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poImage.h"

class poTexture 
	: public poResource
{
public:
	poTexture();
	poTexture(poImage *img);
	poTexture(const std::string &str);
	poTexture(GLenum format, uint width, uint height, uint pitch, ubyte const*pixels);
	poTexture(GLenum format, GLenum internal_Format, GLenum type,
			  uint width, uint height, uint pitch, ubyte const*pixels);
	poTexture(poImage *img, GLenum min, GLenum mag, GLenum wraps, GLenum wrapt);
	~poTexture();
	
	poTexture *copy();
	void replace(ubyte const*pixels);
	
	bool opaqueAtPoint(poPoint p) const;
	poColor colorAtPoint(poPoint p) const;

	uint uid() const;
	uint width() const;
	uint height() const;
	uint pitch() const;
	float s() const;
	float t() const;
	GLenum format() const;
	GLenum internalFormat() const;
	GLenum type() const;
	GLenum minFilter() const;
	GLenum magFilter() const;
	GLenum wrapS() const;
	GLenum wrapT() const;
	
	bool isOnCard() const;
	bool storingPixels() const;
	void deleteLocalMemory();
	void pushToCard();
	void pullFromCard();
	
	void bind(uint unit=0);
	void unbind(uint unit=0);
	
	void st(float s, float t);

protected:
	void createRefCounter();
	void incrRefCount();
	void decrRefCount();
	uint refCount();

private:
	void load(poImage *img);
	void load(poImage *img, GLenum min, GLenum mag, GLenum wraps, GLenum wrapt);
	void loadNotFound();
	void load(GLenum format, GLenum internal_format, GLenum type, 
			  GLenum min, GLenum mag, GLenum ws, GLenum wt,
			  uint w, uint h, uint p, ubyte const*pixels);

	GLuint _uid;
	float _s, _t;
	uint _width, _height, _pitch;
	GLenum _format, _internal_format, _type;
	GLenum _min_filter, _mag_filter;
	GLenum _wrap_s, _wrap_t;
	uint _mem_size;
	GLubyte *_pixels;
	uint *ref_count;
};

GLenum formatForBitDepth(ImageBitDepth bpp);

class poTextureAtlas 
{
public:
	poTextureAtlas(GLenum f, uint w, uint h);
	virtual ~poTextureAtlas();

	// move the list of images to pack
	void clearImages();
	// pass in the id you want to have associated with the image
	void addImage(poImage *img, uint requested_id);
	// will redo all the bin packing with the current set of images
	void layoutAtlas();
	
	bool hasUID(uint uid);
	int numPages();
	poPoint dimensions() const;
	
	uint pageForUID(uint uid);
	poRect coordsForUID(uint uid);
	poRect sizeForUID(uint uid);
	poTexture *textureForPage(uint pg);
	
	// start drawing sets up the texture state
	void startDrawing(uint unit=0);
	// draws will shift the texture as needed,
	// tho user can look at the pages for what it wants to draw and
	// organize it so there are minimal texture switches
	// size should be between 0..1, will scale
	void drawUID(uint uid, poRect r);
	void drawUID(uint uid, poPoint p);
	// reset the texture state to what it was
	void stopDrawing();
	
private:
	void clearPages();
	void bindPage(uint page);
	
	struct ImageLookup {
		uint page;
		poRect coords;
		poPoint size;
	};
	
	int width, height;
	std::vector<ImageLookup> coords;
	// the items to be atlased
	std::vector<poImage*> images;
	// users control the ids
	std::vector<uint> requested_ids;
	// the pages of the atlas
	std::vector<poImage*> pages;
	// the textures of the atlas
	std::vector<poTexture*> textures;
	std::map<uint,uint> uids;
	// the is the format we want
	GLenum format;
	// store the last bound page so we can avoid rebinding
	int bound_page, unit;
};

