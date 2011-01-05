/*
 *  poVectorFont.h
 *  poTestApp
 *../poFramework3/libs/include
 *  Created by Jared Schiffman on 7/21/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#ifndef _PO_VECTOR_FONT_H_
#define _PO_VECTOR_FONT_H_

#include "poFont.h"
#include <ft2build.h>
#include FT_FREETYPE_H



#include "poCommon.h"
#include "poShape2D.h"

class poVectorFont : public poFont
{
public:
	poVectorFont( const char* filename, int size, int faceidx, int charmap );
	
	void			draw();
	virtual void	drawChar(char c, float x, float y, float scale=1.0);
	virtual void	drawString(const char* str, float x, float y, float scale=1.0) { };
	
	virtual int		getCharWidth(char c);
	virtual int		getSpacing(char left, char right);
	virtual int		getLeading() { return height; };
	virtual int		getAscender() { return ascender; };
	virtual int		getDescender() { return descender; };
	
	virtual void construct() { };
	virtual void layoutFont(float x, float y) { };
		
	virtual poObject* getCharMapObj(){ return NULL; }
	
	poShape2D*			letterShapeSet[512];
	FT_Glyph_Metrics	letterMetricsSet[512];
	
	static FT_Library*	library;
	FT_Face				face;
	
	int ascender;
	int descender;
	int height;
};

#endif