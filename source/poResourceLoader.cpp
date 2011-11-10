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

static std::map<ResourceLocator, poImage> imageResources;
static std::map<ResourceLocator, poFont> fontResources;
static std::map<ResourceLocator, poBitmapFont> bitmapFontResources;

template <typename T>
void deleteResourceGroup(std::map<ResourceLocator,T> &map, int group) {
	typename std::map<ResourceLocator,T>::iterator iter = map.begin();
	while(iter != map.end()) {
		if(iter->first.second == group) {
			map.erase(iter++);
		}
		else
			iter++;
	}
}

poImage getImage(const std::string &url, int group) {
	ResourceLocator lookup = std::make_pair(string_hasher(url), group);
	std::map<ResourceLocator, poImage>::iterator iter = imageResources.find(lookup);

	if(iter != imageResources.end()) {
		return iter->second;
	}

	imageResources[lookup] = poImage(url);
	return imageResources[lookup];
}

void deleteImageGroup(int group) {
	deleteResourceGroup(imageResources, group);
}

poFont getFont(const std::string &url, const std::string &style, int group) {
	ResourceLocator lookup = std::make_pair(string_hasher(url+style), group);
	std::map<ResourceLocator, poFont>::iterator iter = fontResources.find(lookup);
	
	if(iter != fontResources.end()) {
		return iter->second;
	}
	
	fontResources[lookup] = poFont(url, style);
	return fontResources[lookup];
}

void deleteFontGroup(int group) {
	deleteResourceGroup(fontResources, group);
}

poBitmapFont getBitmapFont(poFont font, uint size, int group) {
	size_t hash = 0;
	
	std::string combined = font.getFamilyName() + font.getStyleName();
	boost::hash_combine(hash, combined);
	boost::hash_combine(hash, size);
	ResourceLocator lookup = std::make_pair(hash, group);
	std::map<ResourceLocator, poBitmapFont>::iterator iter = bitmapFontResources.find(lookup);
	
	if(iter != bitmapFontResources.end()) {
		return iter->second;
	}
	
	bitmapFontResources[lookup] = poBitmapFont(font, size);
	return bitmapFontResources[lookup];
}

void deleteBitmapFontGroup(int group) {
	deleteResourceGroup(bitmapFontResources, group);
}

