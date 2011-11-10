#pragma once

#include "poFont.h"
#include "poTextureAtlas.h"


// CLASS NOTES
//
// this is really a texture atlas with a different name
//
// to draw a character:
//		bitmapFont->drawUID('a', poPoint(0,0));
//
// to see if a character is cached already:
//		bitmapFont->hasUID('a');
//
// to add a codepoint to the cache:
//		bitmapFont->cacheUID('a');
// 

class poBitmapFont : public poFont {

public:
	poBitmapFont();
	poBitmapFont(poFont font, int pointSize);
	poBitmapFont(const std::string &fam, int pointSize, const std::string &style="");
    virtual ~poBitmapFont();

	void	drawGlyph(int glyph, const poPoint &at);
	
private:
	void	setPointSize(int size) {}
	void	cacheGlyph(int glyph);
	
	poTextureAtlas atlas;
};
