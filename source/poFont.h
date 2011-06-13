//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poRect.h"
#include "poImage.h"
#include "poTexture.h"
#include "poResource.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class poShape2D;

enum poFontTraits {
	PO_FONT_REGULAR		= 0,
	PO_FONT_ITALIC		= 1 << 1,
	PO_FONT_BOLD		= 1 << 2,
};

class poFont
:	public poResource
{
public:
	// pass in a family name or a font url
	poFont(const std::string &family_or_url, int pointSize, poFontTraits traits=PO_FONT_REGULAR);
	virtual ~poFont();
	
	bool valid() const;
	poFont *copy() const;
	
	std::string familyName() const;
	std::string styleName() const;
	// in case you want the same font with different traits
	std::string url() const;
	
	int pointSize() const;
	void pointSize(int size);

	float lineHeight() const;
	float underlinePosition() const;
	float underlineThickness() const;

	int glyph() const;
	void glyph(int g);
	// uses the previously set glyph
	poRect glyphBounds();
	poPoint glyphBearing();
	float	glyphBaselineOffset();
	poPoint glyphAdvance();
	// you have to delete the results
	poImage *glyphImage();
	poShape2D *glyphOutline();

	poPoint kernGlyphs(int glyph1, int glyph2);
	
	std::string toString() const;

private:
	poFont();
	void init();
	void loadGlyph(int g);
	
	static FT_Library lib;
	boost::shared_ptr<FT_FaceRec_> face;

	std::string _url;
	int size, _glyph;
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

class poBitmapFontAtlas : public poTextureAtlas {
public:
	// if -1, it'll store the current point size of the font
	poBitmapFontAtlas(poFont *font, int pointSize=-1);
	virtual ~poBitmapFontAtlas();
	
	void cacheGlyph(uint glyph);
	
private:
	int size;
	poFont *font;
};

std::ostream &operator<<(std::ostream &o, const poFont &f);
