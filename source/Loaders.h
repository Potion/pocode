#pragma once

#include "poFont.h"
#include "poImage.h"

#pragma mark - poFontSpec

class poFontSpec {
public:
	poFontSpec(const std::string &name_or_url, int size, const std::string &weight=PO_FONT_REGULAR)
	: nameOrUrl(name_or_url), pointSize(size), weight(weight)
	{}
	
	bool operator<(const poFontSpec &rhs) const {
		if(nameOrUrl < rhs.nameOrUrl)
			return true;
		if(weight < rhs.weight)
			return true;
		return pointSize < rhs.pointSize;
	}
	
	std::string nameOrUrl;
	std::string weight;
	int pointSize;
};

#pragma mark poFontLoader

class poFontLoader {
public:
	typedef poFontSpec Key;
	typedef poFont Obj;
	
	static poFont not_found;
	
	poFont *load(poFontSpec config) {
		if(fontExists(config.nameOrUrl))
			return new poFont(config.nameOrUrl, config.pointSize, config.weight);
		return NULL;
	}
};


#pragma mark - poImageSpec

class poImageSpec {
public:
	poImageSpec(const std::string &url_or_key)
	:	url(url_or_key), bpp(NUM_IMAGE_DEPTHS), pixels(NULL), width(0), height(0)
	{}
	
	poImageSpec(const std::string &url_or_key, ImageBitDepth bpp)
	:	url(url_or_key), bpp(bpp), pixels(NULL), width(0), height(0)
	{}
	
	poImageSpec(const std::string &url_or_key, 
				  uint w, uint h, ImageBitDepth bpp, const ubyte *pixels)
	:	url(url_or_key), width(w), height(h), bpp(bpp), pixels(pixels)
	{}
	
	bool operator<(const poImageSpec &rhs) const {
		return url < rhs.url;
	}
	
	std::string url;
	
	ImageBitDepth bpp;
	
	int width, height;
	const ubyte *pixels;
};

#pragma mark poImageLoader

class poImageLoader {
public:
	typedef poImageSpec Key;
	typedef poImage Obj;
	
	static poImage not_found;
	
	poImage* load(const poImageSpec &config) {
		poImage *img = NULL;

		if(config.pixels != NULL || (config.width > 0 && config.height > 0))
			img = new poImage(config.width, config.height, config.bpp, config.pixels);
		else if(config.bpp != NUM_IMAGE_DEPTHS)
			img = new poImage(config.url, config.bpp);
		else
			img = new poImage(config.url);
		
		if(!img->isValid()) {
			delete img;
			img = NULL;
			
			return &not_found;
		}
		
		return img;
	}
};

