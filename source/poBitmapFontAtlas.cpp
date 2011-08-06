#include "poBitmapFontAtlas.h"
#include "poResourceStore.h"

poBitmapFontAtlas *getBitmapFont(poFont *font, int siz) {
	return poBitmapFontAtlasCache::instance()->get(font,siz);
}

poBitmapFontAtlasCache::poBitmapFontAtlasCache() {}

poBitmapFontAtlasCache::~poBitmapFontAtlasCache() {
	removeAll();
}

poBitmapFontAtlasCache *poBitmapFontAtlasCache::instance() {
	static boost::shared_ptr<poBitmapFontAtlasCache> inst(new poBitmapFontAtlasCache());
	return inst.get();
}

size_t poBitmapFontAtlasCache::makeHash(poFont *font, int size) {
	size_t hash = 0;
	boost::hash_combine(hash, font);
	boost::hash_combine(hash, size);
	return hash;
}

poBitmapFontAtlas *poBitmapFontAtlasCache::get(poFont *font, int size) {
	if(size > 0)
		font->pointSize(size);
	
	size_t hash = makeHash(font, font->pointSize());

	if(fonts.find(hash) != fonts.end()) {
		return fonts[hash];
	}
	
	fonts[hash] = new poBitmapFontAtlas(font);
	return fonts[hash];
}

void poBitmapFontAtlasCache::remove(poFont *font) {
	size_t hash = makeHash(font, font->pointSize());
	
	if(fonts.find(hash) != fonts.end()) {
		delete fonts[hash];
		fonts.erase(hash);
	}
}

void poBitmapFontAtlasCache::remove(poFont *font, int size) {
	font->pointSize(size);
	remove(font);
}

void poBitmapFontAtlasCache::removeAll() {
	FontMap::iterator i = fonts.begin();
	while(i != fonts.end()) {
		delete i->second;
		i++;
	}
}



poBitmapFontAtlas::poBitmapFontAtlas(poFont *f, int pointSize)
:	poTextureAtlas(poTextureConfig(GL_ALPHA).setMagFilter(GL_LINEAR).setMinFilter(GL_LINEAR),512,512)
,	_font(f)
,	size(pointSize)
{
	if(size == 0)
		size = _font->pointSize();

	_font->pointSize(size);
	
	for(int i=32; i<128; i++) {
		_font->glyph(i);
		
		poImage *img = _font->glyphImage();
		addImage(img, i);
		layoutAtlas();
		
		delete img;
	}
}

poBitmapFontAtlas::~poBitmapFontAtlas() {}

void poBitmapFontAtlas::cacheGlyph(uint glyph) {
	if(!hasUID(glyph)) {
		// if we're in the middle of drawing we have to do this to avoid a flash in the texture
		int bound_page, bound_unit;
		bool bound = bindInfo(&bound_page, &bound_unit);
		if(bound)
			stopDrawing();
		
		_font->pointSize(size);
		_font->glyph(glyph);

		poImage *img = _font->glyphImage();
		addImage(img, glyph);
		layoutAtlas();
		
		delete img;
		
		if(bound)
			startDrawing(bound_unit);
	}
}

poFont const *poBitmapFontAtlas::font() {return _font;}
poPoint poBitmapFontAtlas::originAdjust() {return -poPoint(GLYPH_PADDING, GLYPH_PADDING);}
