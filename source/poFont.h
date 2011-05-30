//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poResource.h"

class poShape2D;

enum {
	FONT_REGULAR	= 0,
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

class poFontMap {
public:
	static const std::string REGULAR_FONT_KEY;
	static const std::string ITALIC_FONT_KEY;
	static const std::string BOLD_FONT_KEY;
	
	bool hasFont(const std::string &name) const;
	poFont *font(const std::string &name) const;
	poFontMap &font(const std::string &name, poFont *font);
	
private:
	std::map<std::string,poFont*> fonts;
	poResourceStore resources;
};

