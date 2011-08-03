#pragma once

#include "poFont.h"
#include "poTextureAtlas.h"

class poBitmapFontAtlas : public poTextureAtlas {
	friend class poFont;
	
public:
	void cacheGlyph(uint glyph);
	// you can't change the font from here
	poFont const *font();
	
private:
	// if -1, it'll store the current point size of the font
	// it does not own the font, make sure the font stays in scope
	poBitmapFontAtlas(poFont *font, int pointSize=-1);
	virtual ~poBitmapFontAtlas();

	int size;
	poFont *_font;
};
