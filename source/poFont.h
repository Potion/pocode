//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poResource.h"

class poShape2D;

enum {
	FONT_ITALIC		= 1,
	FONT_BOLD		= 2,
	FONT_EXPANDED	= 4,
	FONT_CONDENSED	= 8,
	FONT_MONO		= 16,
};

class poFont
:	public poResource
{
public:
	// system font
	poFont(const std::string &family, int traits, float point_size);
	// file-based font
	poFont(const std::string &url, float point_size);
	virtual ~poFont();
	
	poFont *copy();
	
	std::string name() const;
	float size() const;
	// misleading: this is only 1 character, but potentially several bytes long
	poShape2D *getGlyphOutline(const std::string &str);
	
	void *osFontHandle() const;
	
private:
	poFont();
	
	struct poFontImpl;
	poFontImpl *impl;
};