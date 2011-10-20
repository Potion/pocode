//
//  poResourceLoader.h
//  potionCode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poFont.h"
#include "poImage.h"
#include "poBitmapFont.h"

poFont		getFont(const std::string &url, const std::string &style="", int group=-1);
void		deleteFontGroup(int group);

poBitmapFont getBitmapFont(poFont font, uint size, int group=-1);
void		deleteBitmapFontGroup(int group);

poImage		getImage(const std::string &url, int group=-1);
void		deleteImageGroup(int group);
