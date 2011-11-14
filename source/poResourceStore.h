//
//  poResourceLoader.h
//  potionCode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include <map>
#include <typeinfo>
#include "poHelpers.h"

class poFont;
class poBitmapFont;
class poTexture;

// simple helper functions to retreive the resources you want
poFont *poGetFont(const std::string &url, int group=-1);
poFont *poGetFont(const std::string &url, const std::string &style, int group=-1);
poBitmapFont *poGetBitmapFont(poFont* font, uint size, int group=-1);
poBitmapFont *poGetBitmapFont(const std::string &url, uint size, int group=-1);
poBitmapFont *poGetBitmapFont(const std::string &family, const std::string &style, uint size, int group=-1);
poTexture *poGetTexture(const std::string &url, int group=-1);
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
	poResourceLocator(size_t hash, int group, const std::type_info &type)
	:	hash(hash)
	,	group(group)
	,	type(type)
	{}
	
	// must define to make this usable in a std::map
	bool operator<(const poResourceLocator &rhs) const {
		if(type.before(rhs.type))
			return true;
		
		if(group < rhs.group)
			return true;
		
		if(hash < rhs.hash)
			return true;
		
		return false;
	}
};

// base class for resources
class poResource {
public:
	virtual ~poResource()		{}

	void resourceMarkUsed()		{timestamp = poGetElapsedTime();}
	double resourceLastUsed()	{return timestamp;}
	
private:
	double timestamp;
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
	poResourceLocator locatorForFont(const std::string &url, const std::string &style="", int group=-1);
	poResourceLocator locatorForBitmapFont(poFont* font, uint size, int group=-1);
	poResourceLocator locatorForBitmapFont(const std::string &url, uint size, int group=-1);
	poResourceLocator locatorForBitmapFont(const std::string &family, const std::string &style, uint size, int group=-1);
	poResourceLocator locatorForTexture(const std::string &url, int group=-1);
	// pull up a poResource* based on locator
	poResource *findResource(poResourceLocator locator);
	// add any old poResource* subclass you'd like
	void addResource(poResourceLocator locator, poResource *resource);
	// delete a resource group
	void deleteResourceGroup(int group);
	// delete all resources of a given type
	void deleteAllResourcesOfType(const std::type_info &type);
	
private:
	poResourceStore();
	std::map<poResourceLocator, poResource*> resources;
};




