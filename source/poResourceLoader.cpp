//
//  potionCode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poResourceLoader.h"
#include <boost/functional/hash.hpp>

static boost::hash<std::string> string_hasher;

// first=hash, second=group
typedef std::pair<size_t, int> ResourceLocator;

typedef boost::unordered_map<ResourceLocator, poResource*> ResourceMap;
static ResourceMap imageResources;
static ResourceMap fontResources;
static ResourceMap bitmapFontResources;

void deleteResourceGroup(ResourceMap &map, int group) {
	ResourceMap::iterator iter = map.begin();
	while(iter != map.end()) {
		if(iter->first.second == group) {
			delete iter->second;
			map.erase(iter++);
		}
		else
			iter++;
	}
}

poImage *getImage(const std::string &url, int group) {
	ResourceLocator lookup = std::make_pair(string_hasher(url), group);
	
	ResourceMap::iterator iter = imageResources.find(lookup);
	if(iter != imageResources.end()) {
		return (poImage*)iter->second;
	}
	
	poImage *img = new poImage(url);
	imageResources[lookup] = img;
	return img;
}

void deleteImageGroup(int group) {
	deleteResourceGroup(imageResources, group);
}

poFont *getFont(const std::string &url, const std::string &style, int group) {
	ResourceLocator lookup = std::make_pair(string_hasher(url+style), group);
	
	ResourceMap::iterator iter = fontResources.find(lookup);
	if(iter != fontResources.end()) {
		return (poFont*)iter->second;
	}
	
	poFont *font = new poFont(url, style);
	fontResources[lookup] = font;
	return font;
}

void deleteFontGroup(int group) {
	deleteResourceGroup(fontResources, group);
}

poBitmapFont *getBitmapFont(poFont *font, uint size, int group) {
	size_t hash = 0;
	boost::hash_combine(hash, (size_t)font);
	boost::hash_combine(hash, size);
	ResourceLocator lookup = std::make_pair(hash, group);
	
	ResourceMap::iterator iter = bitmapFontResources.find(lookup);
	if(iter != fontResources.end()) {
		return (poBitmapFont*)iter->second;
	}
	
	poBitmapFont *bitmapFont = new poBitmapFont(font, size);
	bitmapFontResources[lookup] = bitmapFont;
	return bitmapFont;
}

void deleteBitmapFontGroup(int group) {
	deleteResourceGroup(bitmapFontResources, group);
}



