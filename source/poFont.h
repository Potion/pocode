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
class poFontLoader;
class poBitmapFont;

class poFont;
typedef std::map<std::string, poFont*> poFontMap;

// CLASS NOTES
//
// A poFont implements general font functionality. A single poFont object represents a single
// font face in a single font size.
//
// You cannot construct a new font directly. You can load a new font as follows:
//
//      poFont* font = getFont("Courier", 20);
//
// Using getFont() ensures that the same font is not loaded and constructed multiple times.
//
// 

class poFont {
	friend std::ostream &operator<<(std::ostream &o, const poFont *f);

public:
	static bool fontExists(const std::string &family_or_url);
	
	poFont();
	poFont(const std::string &family_or_url, const std::string &style="");
	virtual ~poFont();
	
    // FONT LOADING
	bool                isValid() const;
	
    // FONT PROPERTIES
	std::string         getFamilyName() const;
	std::string         getStyleName() const;
	std::string         getUrl() const;
	bool                hasKerning() const;
	
	int                 getPointSize() const;
	void                setPointSize(int size);

	float               getLineHeight() const;
	float               getAscender() const;
	float               getDescender() const;
    
    // UNDERLINE
	// maximum bbox for this font face at this size
	float               getUnderlinePosition() const;
	float               getUnderlineThickness() const;

    // CURRENT GLYPH
    // Get and set the current glyph.
	int                 getGlyph() const;
	void                setGlyph(int g);
    
	// These functions (starting with 'glyph') return info about the current codepoint.
	poRect              getGlyphBounds() const;
	poRect              getGlyphFrame() const;
	float               getGlyphDescender() const;
	poPoint             getGlyphBearing() const;
	poPoint             getGlyphAdvance() const;
	poImage             getGlyphImage() const;
//	poShape2D           *getGlyphOutline() const;

	poPoint             kernGlyphs(int glyph1, int glyph2) const;

	std::string         toString() const;

private:
	void                loadGlyph(int g);
	
	struct FontImpl {
		FontImpl();
		FontImpl(const std::string &family_or_url, const std::string &style="");
		~FontImpl();
		
		std::string			url;
		int					size;
		int					glyph;
		FT_Face				face;
		static FT_Library   lib;
	};
	boost::shared_ptr<FontImpl> shared;
};

