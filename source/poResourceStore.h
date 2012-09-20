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
//  poResourceLoader.h
//  pocode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include <map>
#include <typeinfo>
#include "poHelpers.h"
#include "poFileLocation.h"

class poFont;
class poBitmapFont;
class poTexture;

// simple helper functions to retreive the resources you want
poFont *poGetFont(const poFilePath &filePath, int group=0);
poFont *poGetFont(const poFilePath &filePath, const std::string &style, int group=0);

poFont *poGetFontByName(const std::string &family, int group=0);
poFont *poGetFontByName(const std::string &family, const std::string &style, int group=0);

poBitmapFont *poGetBitmapFont(poFont* font, uint size, int group=0);
poBitmapFont *poGetBitmapFont(const poFilePath &filePath, uint size, int group=0);

poBitmapFont *poGetBitmapSystemFont(const std::string &family, const std::string &style, uint size, int group=0);

poTexture *poGetTexture(const poFilePath &filePath, bool keepImage=false, int group=0);

void poDeleteResourceGroup(int group);
void poDeleteResourceType(const std::type_info &type);

// helper class to find stored resources 
struct poResourceLocator {
	// each class type should have a consisten hashing method
	size_t hash;
	// resource groups let you delete a batch
	int group;
	// type_info keeps resources distinct from eachother
	const std::type_info &type;
	
	// this is the only way to make one of these
	poResourceLocator(size_t h, int g, const std::type_info &t);
	// must define to make this usable in a std::map
	bool operator<(const poResourceLocator &rhs) const;
};
inline std::ostream &operator<<(std::ostream &out, poResourceLocator const& loc) {
	out << "resource(" << loc.hash << "," << loc.group << "," << loc.type.name() << ")";
	return out;
}

// base class for resources
class poResource {
public:
	virtual ~poResource()		{}

	void resourceMarkUsed()		{timestamp = poGetElapsedTime();}
	float resourceLastUsed()	{return timestamp;}
	
private:
	float timestamp;
};

// helper function to retrieve the type_info of a given poResource subclass
inline const std::type_info &poResourceType(poResource *rez) {
	return typeid(*rez);
}

// a place to stash pointers to resources
// to facilitate reuse and help managed memory
class poResourceStore {
public:
	// grab the singleton
	static poResourceStore *get();
	
	// build a locator to find a resource of a given type
	poResourceLocator locatorForFont(const poFilePath &filePath, const std::string &style="", int group=-1);
	poResourceLocator locatorForBitmapFont(poFont* font, uint size, int group=-1);
	poResourceLocator locatorForBitmapFont(const poFilePath &filePath, uint size, int group=-1);
	poResourceLocator locatorForBitmapFont(const std::string &family, const std::string &style, uint size, int group=-1);
    
	poResourceLocator locatorForTexture(const poFilePath &filePath, int group=-1);
	// pull up a poResource* based on locator
	poResource *findResource(poResourceLocator locator);
	// add any old poResource* subclass you'd like
	void addResource(poResourceLocator locator, poResource *resource);
	// delete a resource group
	void deleteResourceGroup(int group);
	// delete all resources of a given type
	void deleteAllResourcesOfType(const std::type_info &type);
    bool resourceIsCached(poResource* rez);
	
private:
	poResourceStore();
	std::map<poResourceLocator, poResource*> resources;
};




