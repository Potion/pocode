#include "poBitmapFont.h"

poBitmapFont::poBitmapFont()
:	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
{}

poBitmapFont::poBitmapFont(poFont *font, int sz)
:	poFont(font->getFamilyName(), font->getStyleName())
,	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		
		poImage *img = getGlyphImage();
		atlas->addImage(img,i);
		delete img;
	}
	atlas->layoutAtlas();
}

poBitmapFont::poBitmapFont(const std::string &fam, int sz, const std::string &style)
:	poFont(fam, style)
,	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		
		poImage *img = getGlyphImage();
		atlas->addImage(img,i);
		delete img;
	}
	atlas->layoutAtlas();
}

void poBitmapFont::drawGlyph(int glyph, const poPoint &at) {
	if(!atlas->hasUID(glyph))
		cacheGlyph(glyph);
	atlas->drawUID(glyph, at);
}

void poBitmapFont::cacheGlyph(int glyph) {
	if(!atlas->hasUID(glyph)) {
		setPointSize(getPointSize());
		setGlyph(glyph);
		
		poImage *img = getGlyphImage();
		atlas->addImage(img, glyph);
		delete img;
		
		atlas->layoutAtlas();
	}
}
