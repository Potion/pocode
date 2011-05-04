//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poResource.h"

class poShape2D;

class poFont
:	public poResource
{
public:
	poFont(const std::string &font_name, float point_size);
	virtual ~poFont();
	
	poFont *copy();
	
	std::string name() const;
	float size() const;
	// misleading: this is only 1 character, but potentially several bytes long
	poShape2D *getGlyphOutline(const std::string &str);
	
private:
	poFont();
	
	struct poFontImpl;
	poFontImpl *impl;
};