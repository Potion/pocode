#include "poBitmapFont.h"

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
		delete img;
	}
	
	layoutAtlas();
}

poFont const *poBitmapFont::font() {
    return _font;
}

uint poBitmapFont::pointSize() const {
	return size;
}

void poBitmapFont::cacheUID(uint codepoint) {
	if(!hasUID(codepoint)) {
		_font->pointSize(size);
		_font->glyph(codepoint);

		poImage *img = _font->glyphImage();
		addImage(img, codepoint);
		delete img;

		layoutAtlas();
	}
}

