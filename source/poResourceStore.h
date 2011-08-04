//
//  poResourceStore.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poFont.h"
#include "poImage.h"

#include "Loaders.h"
#include "ResourceStore.h"

#define DEFAULT_REZ_GROUP -1

poFont *getFont(); // the not_found image
poFont *getFont(const std::string &name_or_url, int size, const std::string &style="Regular", int gid=DEFAULT_REZ_GROUP);

poImage *getImage(); // the not_found image
poImage *getImage(const std::string &url_or_key, int gid=DEFAULT_REZ_GROUP);
poImage *getImage(const std::string &url_or_key, ImageBitDepth bpp, int gid=DEFAULT_REZ_GROUP);
poImage *getImage(const std::string &url_or_key, uint w, uint h, ImageBitDepth bpp, const ubyte *pixels, int gid=DEFAULT_REZ_GROUP);



static poResourceStore<poFontLoader> *fontStore() {
	using namespace boost;
	static shared_ptr< poResourceStore<poFontLoader> > store;
	if(!store)
		store.reset(new poResourceStore<poFontLoader>());
	return store.get();
}

static poResourceStore<poImageLoader>* imageStore() {
	using namespace boost;
	static shared_ptr< poResourceStore<poImageLoader> > store;
	if(!store)
		store.reset(new poResourceStore<poImageLoader>());
	return store.get();
}

