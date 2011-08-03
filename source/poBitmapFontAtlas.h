#pragma once

#include "poFont.h"
#include "Loaders.h"
#include "poTextureAtlas.h"

// this is the right way to get a bitmap font
poBitmapFontAtlas *getBitmapFont(poFont *font, int size=0);

class poBitmapFontAtlasCache {
public:
	static poBitmapFontAtlasCache *instance();
	~poBitmapFontAtlasCache();

	poBitmapFontAtlas *get(poFont *font, int size=0);
	
	void remove(poFont *font);
	void remove(poFont *font, int size);
	void removeAll();
	
private:
	typedef std::map<size_t,poBitmapFontAtlas*> FontMap;
	
	explicit poBitmapFontAtlasCache();
	
	size_t makeHash(poFont *font, int size);
	
	FontMap fonts;
};

class poBitmapFontAtlas : public poTextureAtlas {
	friend class poBitmapFontAtlasCache;
	
public:
	void cacheGlyph(uint glyph);
	// you can't change the font from here
	poFont const *font();
	
private:
	// if -1, it'll store the current point size of the font
	// it does not own the font, make sure the font stays in scope
	poBitmapFontAtlas(poFont *font, int pointSize=0);
	virtual ~poBitmapFontAtlas();

	int size;
	poFont *_font;
};
