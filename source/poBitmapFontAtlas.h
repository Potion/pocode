#pragma once

#include "poFont.h"
#include "poTextureAtlas.h"

class poBitmapFontAtlas : public poTextureAtlas {
public:
	// if -1, it'll store the current point size of the font
	poBitmapFontAtlas(poFont *font, int pointSize=-1);
	virtual ~poBitmapFontAtlas();
	
	void cacheGlyph(uint glyph);
	// you can't change the font from here
	poFont const *font();
	
private:
	int size;
	poFont *_font;
};
