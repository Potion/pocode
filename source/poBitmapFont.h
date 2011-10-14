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

class poBitmapFont : public poTextureAtlas {
public:
	poBitmapFont(poFont *font, int pointSize=0);
	
	poFont const* font();

	uint pointSize() const;
	void cacheUID(uint codepoint);

private:
	int size;
	poFont *_font;
};

