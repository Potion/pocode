#include "poBitmapFontAtlas.h"


poBitmapFontAtlas::poBitmapFontAtlas(poFont *f, int pointSize)
:	poTextureAtlas(poTextureConfig(GL_ALPHA).setMagFilter(GL_LINEAR).setMinFilter(GL_LINEAR),512,512)
,	_font(f)
,	size(pointSize)
{
	if(size < 0)
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
		_font->pointSize(size);
		_font->glyph(glyph);

		poImage *img = _font->glyphImage();
		addImage(img, glyph);
		layoutAtlas();
		
		delete img;
	}
}

poFont const *poBitmapFontAtlas::font() {return _font;}
