#include "poBitmapFont.h"


poBitmapFont::poBitmapFont()
:	font(NULL)
,	atlas(NULL)
,	pointSize(0)
{}

poBitmapFont::poBitmapFont(poFont* font, int sz)
:	font(font)
,	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
,	pointSize(sz)
{
	init();
}

poBitmapFont::poBitmapFont(const std::string &url, int sz)
:	font(poGetFont(url))
,	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
,	pointSize(sz)
{
	init();
}

poBitmapFont::poBitmapFont(const std::string &fam, const std::string &style, int sz)
:	font(poGetFont(fam,style))
,	atlas(new poTextureAtlas(GL_ALPHA, 512, 512))
,	pointSize(sz)
{
	init();
}

void poBitmapFont::init() {
	font->setPointSize(pointSize);
	for(int i=32; i<128; i++) {
		font->setGlyph(i);
		
		poImage *img = font->getGlyphImage();
		atlas->addImage(img,i);
		delete img;
	}
	atlas->layoutAtlas();
}

poBitmapFont::~poBitmapFont() {
	delete atlas;
}

poFont *poBitmapFont::getFont() const {
	return font;
}

int poBitmapFont::getPointSize() const {
	return pointSize;
}

void poBitmapFont::drawGlyph(int glyph, const poPoint &at) {
	if(!atlas->hasUID(glyph))
		cacheGlyph(glyph);
	atlas->drawUID(glyph, at);
}

void poBitmapFont::cacheGlyph(int glyph) {
	if(!atlas->hasUID(glyph)) {
		font->setPointSize(getPointSize());
		font->setGlyph(glyph);
		
		poImage* img = font->getGlyphImage();
		atlas->addImage(img, glyph);
		delete img;
		
		atlas->layoutAtlas();
	}
}


