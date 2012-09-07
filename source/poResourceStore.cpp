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
//  pocode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poResourceStore.h"
#include <boost/functional/hash.hpp>

poFont *poGetFont(const poFilePath &filePath, int group) {
	return poGetFont(filePath, "", group);
}

poFont *poGetFont(const poFilePath &filePath, const std::string &style, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForFont(filePath, style, group);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poFont*)found;
	
	poFont *font = new poFont(filePath, style);
	store->addResource(lookup, font);
	return font;
}

poFont *poGetFontByName(const std::string &family, int group) {
	return poGetFontByName(family, "", group);
}

poFont *poGetFontByName(const std::string &family, const std::string &style, int group) {
    poFilePath path;
    urlForFontFamilyName(family, style, path);
    
	return poGetFont(path, "", group);
}


poBitmapFont *poGetBitmapFont(const poFilePath &filePath, uint size, int group) {
    poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForBitmapFont(filePath, size, group);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poBitmapFont*)found;
    
	poBitmapFont* bmpFont = new poBitmapFont(filePath, size);
	store->addResource(lookup, bmpFont);
	return bmpFont;
}

poBitmapFont *poGetBitmapFont(poFont* font, uint size, int group) {
	return poGetBitmapFont(font->getFilePath(), size, group);
}

poBitmapFont *poGetBitmapSystemFont(const std::string &family, const std::string &style, uint size, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForBitmapFont(family, style, size, group);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poBitmapFont*)found;

	poBitmapFont* bmpFont = new poBitmapFont(family, style, size);
	store->addResource(lookup, bmpFont);
	return bmpFont;
}

poTexture *poGetTexture(const std::string &url, bool keepImage, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForTexture(url);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poTexture*)found;
	
	poTexture *tex = new poTexture(url,keepImage);
	store->addResource(lookup, tex);
	return tex;
}

void poDeleteResourceGroup(int group);
void poDeleteResourceType(const std::type_info &type);

// this is the only way to make one of these
poResourceLocator::poResourceLocator(size_t h, int g, const std::type_info &t)
:	hash(h)
,	group(g)
,	type(t)
{
	boost::hash_combine(hash, group);
	boost::hash_combine(hash, type.name());
}

// must define to make this usable in a std::map
bool poResourceLocator::operator<(const poResourceLocator &rhs) const {
	return hash < rhs.hash;
}

static boost::hash<std::string> string_hasher;

poResourceStore *poResourceStore::get() {
	static poResourceStore *instance = NULL;
	if(!instance)
		instance = new poResourceStore;
	return instance;
}

poResourceStore::poResourceStore() {
}

poResourceLocator poResourceStore::locatorForFont(const poFilePath &filePath, const std::string &style, int group) {
	poResourceLocator lookup(0, group, typeid(poFont));
	lookup.hash = string_hasher(filePath.asString() + style);
	return lookup;
}

poResourceLocator poResourceStore::locatorForBitmapFont(const poFilePath &filePath, uint size, int group) {
	return locatorForBitmapFont(poGetFont(filePath, group), size, group);
}

poResourceLocator poResourceStore::locatorForBitmapFont(poFont* font, uint size, int group) {
	return locatorForBitmapFont(font->getFamilyName(), font->getStyleName(), size, group);
}

poResourceLocator poResourceStore::locatorForBitmapFont(const std::string &family, const std::string &style, uint size, int group) {
	poResourceLocator lookup(0, group, typeid(poBitmapFont));
	boost::hash_combine(lookup.hash, family + style);
	boost::hash_combine(lookup.hash, size);
	return lookup;
}

poResourceLocator poResourceStore::locatorForTexture(const std::string &url, int group) {
	poResourceLocator lookup(0, group, typeid(poTexture));
	lookup.hash = string_hasher(url);
	return lookup;
}

void poResourceStore::deleteResourceGroup(int group) {
	std::map<poResourceLocator,poResource*>::iterator iter = resources.begin();
	while(iter != resources.end()) {
		if(iter->first.group == group) {
			delete iter->second;
			resources.erase(iter++);
		}
		else
			iter++;
	}
}

void poResourceStore::deleteAllResourcesOfType(const std::type_info &type) {
	std::map<poResourceLocator,poResource*>::iterator iter = resources.begin();
	while(iter != resources.end()) {
		if(iter->first.type == type) {
			delete iter->second;
			resources.erase(iter++);
		}
		else
			iter++;
	}
}

poResource *poResourceStore::findResource(poResourceLocator locator) {
	std::map<poResourceLocator, poResource*>::iterator iter = resources.find(locator);
	if(iter != resources.end())
		return iter->second;
	return NULL;
}

void poResourceStore::addResource(poResourceLocator locator, poResource *resource) {
	if(!findResource(locator)) {
		resources[locator] = resource;
	}
}

bool poResourceStore::resourceIsCached(poResource *rez) {
	std::map<poResourceLocator, poResource*>::iterator iter = resources.begin();
    while(iter != resources.end()) {
        if(iter->second == rez)
            return true;
        ++iter;
    }
    return false;
    
}

