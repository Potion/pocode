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

#define DEFAULT_REZ_GROUP -1

poFont *getFont(); // the not_found image
poFont *getFont(const std::string &name_or_url, int size, const std::string &weight=PO_FONT_REGULAR, int gid=DEFAULT_REZ_GROUP);

poImage *getImage(); // the not_found image
poImage *getImage(const std::string &url_or_key, int gid=DEFAULT_REZ_GROUP);
poImage *getImage(const std::string &url_or_key, ImageBitDepth bpp, int gid=DEFAULT_REZ_GROUP);
poImage *getImage(const std::string &url_or_key, uint w, uint h, ImageBitDepth bpp, const ubyte *pixels, int gid=DEFAULT_REZ_GROUP);

