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

#include "poBitmapFont.h"
#include "poApplication.h"

namespace po {
    BitmapFont::BitmapFont()
    :	font(NULL)
    ,	atlas(NULL)
    ,	pointSize(0)
    {}

    BitmapFont::BitmapFont(Font* font, int sz)
    :	font(font)
    ,	atlas(new TextureAtlas(GL_ALPHA, 512, 512))
    ,	pointSize(sz)
    {
        init();
    }

    BitmapFont::BitmapFont(const poFilePath &filePath, int sz)
    :	font(poGetFont(filePath))
    ,	atlas(new TextureAtlas(GL_ALPHA, 512, 512))
    ,	pointSize(sz)
    {
        init();
    }

    BitmapFont::BitmapFont(const std::string &family, const std::string &style, int sz)
    :	font(poGetFontByName(family,style))
    ,	atlas(new TextureAtlas(GL_ALPHA, 512, 512))
    ,	pointSize(sz)
    {
        init();
    }

    BitmapFont::~BitmapFont() {
        delete atlas;
    }
    
    
    //------------------------------------------------------------------------
    void BitmapFont::init() {
        font->setPointSize(pointSize);
        for(int i=32; i<128; i++) {
            font->setGlyph(i);
            
            poImage *img = font->getGlyphImage();
            atlas->addImage(img,i);
            delete img;
        }
        atlas->layoutAtlas();
    }
    
    
    //------------------------------------------------------------------------
    Font *BitmapFont::getFont() const {
        return font;
    }
    
    
    //------------------------------------------------------------------------
    int BitmapFont::getPointSize() const {
        return pointSize;
    }
    
    
    //------------------------------------------------------------------------
    void BitmapFont::drawGlyph(int glyph, const Point &at) {
        
        if(!atlas->hasUID(glyph))
            cacheGlyph(glyph);
        atlas->drawUID(glyph, at, po::getScale());
    }
    
    
    //------------------------------------------------------------------------
    void BitmapFont::cacheGlyph(int glyph) {
        if(!atlas->hasUID(glyph)) {
            font->setPointSize(getPointSize());
            font->setGlyph(glyph);
            
            poImage* img = font->getGlyphImage();
            atlas->addImage(img, glyph);
            delete img;
            
            atlas->layoutAtlas();
        }
    }
} /* End po namespace */