#include "poBitmapFont.h"

poBitmapFont::BitmapFontImpl::BitmapFontImpl()
:	atlas(GL_ALPHA, 512, 512)
{}
poBitmapFont::BitmapFontImpl::~BitmapFontImpl() {
}

poBitmapFont::poBitmapFont()
{}

poBitmapFont::poBitmapFont(poFont font, int sz)
:	poFont(font)
,	shared(new BitmapFontImpl)
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		shared->atlas.addImage(getGlyphImage(),i);
	}
	shared->atlas.layoutAtlas();
}

poBitmapFont::poBitmapFont(const std::string &fam, int sz, const std::string &style)
:	poFont(fam, style)
,	shared(new BitmapFontImpl)
{
	poFont::setPointSize(sz);
	for(int i=32; i<128; i++) {
		setGlyph(i);
		shared->atlas.addImage(getGlyphImage(),i);
	}
	shared->atlas.layoutAtlas();
}

void poBitmapFont::drawGlyph(int glyph, const poPoint &at) {
	if(!shared->atlas.hasUID(glyph))
		cacheGlyph(glyph);
	shared->atlas.drawUID(glyph, at);
}

void poBitmapFont::cacheGlyph(int glyph) {
	if(!shared->atlas.hasUID(glyph)) {
		setPointSize(getPointSize());
		setGlyph(glyph);
		shared->atlas.addImage(getGlyphImage(), glyph);
		shared->atlas.layoutAtlas();
	}
}

bool operator==(const poBitmapFont& f1, const poBitmapFont& f2) {
	return f1.shared == f2.shared;
}

bool operator!=(const poBitmapFont& f1, const poBitmapFont& f2) {
	return f1.shared == f2.shared;
}
