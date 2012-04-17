/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

class poBitmapFont : public poResource {

public:
	poBitmapFont();
	poBitmapFont(poFont* font, int pointSize);
	poBitmapFont(const std::string &url, int pointSize);
	poBitmapFont(const std::string &fam, const std::string &style, int pointSize);
	virtual ~poBitmapFont();

	poFont*	getFont() const;
	int		getPointSize() const;

	void	drawGlyph(int glyph, const poPoint &at);
	
private:
	void	init();
	void	cacheGlyph(int glyph);
	
	poFont *font;
	poTextureAtlas *atlas;
	int pointSize;
};
