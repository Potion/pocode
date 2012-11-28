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

//
//  Font.h
//  pocode
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poRect.h"
#include "poImage.h"
#include "poResourceStore.h"

#include <ft2build.h> 
#include FT_FREETYPE_H 
#include FT_BITMAP_H 
#include FT_GLYPH_H 
#include FT_IMAGE_H

namespace po {
    class Shape2D;

    // CLASS NOTES
    //
    // A Font implements general font functionality. A single Font object represents a single
    // font face in a single font size.
    //
    // You cannot construct a new font directly. You can load a new font as follows:
    //
    //      Font* font = getFont("Courier", 20);
    //
    // Using getFont() ensures that the same font is not loaded and constructed multiple times.
    //
    // 

    struct FontGlyphMetrics  {
        Rect  glyphBounds;
        Rect  glyphFrame;
        float   glyphDescender;
        Point glyphBearing;
        Point glyphAdvance;
    };

    typedef std::vector<FontGlyphMetrics> glyphMetricsVector;

    // http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Encoding
    unsigned long encodeTag(const char tag[4]);
    std::string decodeTag(unsigned long encoded);

    bool urlForFontFamilyName(const std::string &family, const std::string &style, FilePath &response);

    class Font : public Resource {
        friend std::ostream &operator<<(std::ostream &o, const Font *f);

    public:
        static bool fontExists(const std::string &family_or_url);
        static Font *defaultFont();
        
        Font();
        Font(const FilePath &filePath, const std::string &style="", unsigned long encoding=encodeTag("unic"));
        virtual ~Font();
        
        // FONT LOADING
        bool                isValid() const;
        
        // FONT PROPERTIES
        std::string         getFamilyName() const;
        std::string         getStyleName() const;
        FilePath          getFilePath() const;
        bool                hasKerning() const;
        std::vector<std::string>
                            getEncodings() const;
        
        int                 getPointSize() const;
        void                setPointSize(int size);

        float               getLineHeight() const;
        float               getAscender() const;
        float               getDescender() const;
        
        // UNDERLINE
        // maximum bbox for this font face at this size
        float               getUnderlinePosition() const;
        float               getUnderlineThickness() const;

        // CURRENT GLYPH
        // Get and set the current glyph.
        int                 getGlyph() const;
        void                setGlyph(int g);
        
        // These functions (starting with 'glyph') return info about the current codepoint.
        Rect              getGlyphBounds();
        Rect              getGlyphFrame();
        float               getGlyphDescender();
        Point             getGlyphBearing();
        Point             getGlyphAdvance();
        Image*			getGlyphImage();
    //	poShape2D           *getGlyphOutline() const;

        Point             kernGlyphs(int glyph1, int glyph2) const;

        std::string         toString() const;
        std::string			getRequestedFamilyName() const;
        std::string			getRequestedStyleName() const;

        bool                cachedForSizeYet(int fontSize);
        void                cacheGlyphMetrics();

    private:
        void                loadGlyph(int g);
        
        glyphMetricsVector                  *currentCache;
        std::map<int,glyphMetricsVector>    cachedGlyphMetricsSet;
        
        FilePath          filePath;
        std::string			reqFamily, reqStyle;
        int					size;
        int					glyph, loadedGlyph;
        FT_Face				face;
        static FT_Library   lib;
    };
} /* End po namespace */