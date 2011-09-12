#pragma once

#include "poFont.h"
#include "Loaders.h"
#include "poTextureAtlas.h"

// This is the right way to get a bitmap font.
poBitmapFont*   getBitmapFont(poFont *font, int size=0);


// CLASS NOTES
//
// Coming soon...
//
// 

class poBitmapFontCache;

class poBitmapFont : public poTextureAtlas {
	friend class    poBitmapFontCache;
	
public:
    // DRAW SINGLE GLYPH
    // Use this method for drawing a single character (glyph) in this font.
    // This is not as efficient as the method below for drawing multiple characters.
    void            drawSingleGlyph( uint glyph, poPoint position );
    
    // DRAW GLYPHS
    // Use these methods for drawing many characters in this font.
    // 1. setUpFont()
    // 2. drawGlyph() (multiple times)
    // 3. setDownFont()
    void            setUpFont();
    void            drawGlyph( uint glyph, poPoint position );
    void            setDownFont();
    
    // SOURCE FONT
	// you can't change the font from here
	poFont const*   font();

private:
	// if -1, it'll store the current point size of the font
	// it does not own the font, make sure the font stays in scope
	poBitmapFont(poFont *font, int pointSize=0);
	virtual ~poBitmapFont();

	virtual poPoint originAdjust();
    void            cacheGlyph(uint glyph);

	int             size;
	poFont          *_font;
};



class poBitmapFontCache {
public:
	static poBitmapFontCache*   instance();
	~poBitmapFontCache();
    
	poBitmapFont*   get(poFont *font, int size=0);
	
	void            remove(poFont *font);
	void            remove(poFont *font, int size);
	void            removeAll();
	
private:
	typedef std::map<size_t,poBitmapFont*> FontMap;
	
	explicit        poBitmapFontCache();
	
	size_t          makeHash(poFont *font, int size);
	
	FontMap         fonts;
};



