#include "poFont.h"


#ifndef _PO_BITMAP_FONT_H_
#define _PO_BITMAP_FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H

class poTexture;
class poImage;

struct poBitmapGlyph 
{
	poTexture* tex;
	poImage* image;
	FT_Glyph_Metrics metrics;
	
	poBitmapGlyph() {
		tex = NULL;
		image = NULL;
	}
};

class poBitmapFont : public poFont
{
public:
	poBitmapFont(const char* fontfile, int size, int faceidx, int charmap);
	~poBitmapFont();

	virtual void		drawChar(char c, float x, float y, float scale=1.0);
	virtual void		drawString(const char* str, float x, float y, float scale=1.0);

	virtual int			getCharWidth(char c);
	virtual int			getStringWidth(const char* str);
	virtual int			getSpacing(char left, char right);
	virtual int			getLeading();
	virtual int			getAscender();
	virtual int			getDescender();

	virtual void		construct();

	virtual void		remapChar(int from, int to);
	virtual void		selectCharMap(int mapIdx);
	virtual void		addAFMfile(const char* afmfile);

	virtual void		layoutFont(float x, float y);
	
	virtual poObject*	getCharMapObj();

	static FT_Library*	library;
	FT_Face				face;

	int ascender;
	int descender;
	int height;
	
	poBitmapGlyph charmap[256];
	//poBitmapGlyph* charmap;
};

#endif