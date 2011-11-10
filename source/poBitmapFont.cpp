#include "poBitmapFont.h"


poBitmapFont::poBitmapFont()
:	atlas(GL_ALPHA, 512, 512)
{}

poBitmapFont::poBitmapFont(poFont font, int sz)
:	poFont(font)
,	atlas(GL_ALPHA, 512, 512)
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		
		poImage img = getGlyphImage();
		atlas.addImage(img,i);
	}
	atlas.layoutAtlas();
}

poBitmapFont::poBitmapFont(const std::string &fam, int sz, const std::string &style)
:	poFont(fam, style)
,	atlas(GL_ALPHA, 512, 512)
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		
		poImage img = getGlyphImage();
		atlas.addImage(img,i);
	}
	atlas.layoutAtlas();
}

poBitmapFont::~poBitmapFont() {
}

void poBitmapFont::drawGlyph(int glyph, const poPoint &at) {
	if(!atlas.hasUID(glyph))
		cacheGlyph(glyph);
	atlas.drawUID(glyph, at);
}

void poBitmapFont::cacheGlyph(int glyph) {
	if(!atlas.hasUID(glyph)) {
		setPointSize(getPointSize());
		setGlyph(glyph);
		
		poImage img = getGlyphImage();
		atlas.addImage(img, glyph);
		
		atlas.layoutAtlas();
	}
}
