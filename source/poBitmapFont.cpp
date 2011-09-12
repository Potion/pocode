#include "poBitmapFont.h"
#include "poResourceStore.h"

poBitmapFont *getBitmapFont(poFont *font, int siz) {
	return poBitmapFontCache::instance()->get(font,siz);
}




poBitmapFont::poBitmapFont(poFont *f, int pointSize)
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

poBitmapFont::~poBitmapFont() {}

void poBitmapFont::cacheGlyph(uint glyph) {
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

void    poBitmapFont::drawSingleGlyph( uint glyph, poPoint position )
{
    startDrawing(0);
    cacheGlyph(glyph);
    drawUID( glyph, position );
    stopDrawing();
}

void    poBitmapFont::drawGlyph( uint glyph, poPoint position )
{
    cacheGlyph( glyph );
    drawUID( glyph, position );
}

void    poBitmapFont::setUpFont()
{
    startDrawing(0);
}

void    poBitmapFont::setDownFont()
{
    stopDrawing();
}

poFont const *poBitmapFont::font()
{
    return _font;
}

poPoint poBitmapFont::originAdjust()
{
    return -poPoint(GLYPH_PADDING, GLYPH_PADDING);
}





poBitmapFontCache::poBitmapFontCache() {}

poBitmapFontCache::~poBitmapFontCache() {
	removeAll();
}

poBitmapFontCache *poBitmapFontCache::instance() {
	static boost::shared_ptr<poBitmapFontCache> inst(new poBitmapFontCache());
	return inst.get();
}

size_t poBitmapFontCache::makeHash(poFont *font, int size) {
	size_t hash = 0;
	boost::hash_combine(hash, font);
	boost::hash_combine(hash, size);
	return hash;
}

poBitmapFont *poBitmapFontCache::get(poFont *font, int size) {
	if(size > 0)
		font->pointSize(size);
	
	size_t hash = makeHash(font, font->pointSize());
    
	if(fonts.find(hash) != fonts.end()) {
		return fonts[hash];
	}
	
	fonts[hash] = new poBitmapFont(font);
	return fonts[hash];
}

void poBitmapFontCache::remove(poFont *font) {
	size_t hash = makeHash(font, font->pointSize());
	
	if(fonts.find(hash) != fonts.end()) {
		delete fonts[hash];
		fonts.erase(hash);
	}
}

void poBitmapFontCache::remove(poFont *font, int size) {
	font->pointSize(size);
	remove(font);
}

void poBitmapFontCache::removeAll() {
	FontMap::iterator i = fonts.begin();
	while(i != fonts.end()) {
		delete i->second;
		i++;
	}
}

