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
//  Font.cpp
//  pocode
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poShape2D.h"
#include "poHelpers.h"
#include "poApplication.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <freetype/ftbitmap.h>
using namespace std;

namespace po {
    #ifdef POTION_WINDOWS

        #include "windows.h"

            bool urlForFontFamilyName(const std::string &family, const std::string &style, poFilePath &response) {
                return false;
            }

    #elif defined(POTION_APPLE)
        #include <CoreFoundation/CoreFoundation.h>

        #if defined(POTION_MAC)
            #include <ApplicationServices/ApplicationServices.h>

            bool urlForFontFamilyName(const std::string &family, const std::string &style, poFilePath &response) {
                CFMutableDictionaryRef attributes = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                
                CFStringRef fam_str = CFStringCreateWithBytes(NULL, (const UInt8*)family.c_str(), family.size(), kCFStringEncodingUTF8, false);
                CFStringRef sty_str = CFStringCreateWithBytes(NULL, (const UInt8*)style.c_str(), style.size(), kCFStringEncodingUTF8, false);
                CFDictionaryAddValue(attributes, kCTFontFamilyNameAttribute, fam_str);
                CFDictionaryAddValue(attributes, kCTFontStyleNameAttribute, sty_str);
                CFRelease(fam_str);
                CFRelease(sty_str);
                
                CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes(attributes);
                CFRelease(attributes);
                
                CFURLRef url = (CFURLRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontURLAttribute);
                CFStringRef styleName = (CFStringRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontStyleNameAttribute);
                CFStringRef displayName = (CFStringRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontDisplayNameAttribute);
                
                CFRelease(descriptor);
                
                if(!url)
                    return false;
                
                UInt8 path[1024];
                CFURLGetFileSystemRepresentation(url, true, path, 1024);
                CFRelease(url);
                
                response.set((char*)path);
                return true;
            }

        #else

            bool urlForFontFamilyName(const std::string &family, const std::string &style, poFilePath &response) {
                return false;
            }

        #endif

    #endif
    FT_Library Font::lib = NULL;

    unsigned long encodeTag(const char tag[4]) {
        unsigned long rez = 0;
        rez |= tag[0] << 24;
        rez |= tag[1] << 16;
        rez |= tag[2] << 8;
        rez |= tag[3];
        return rez;
    }

    std::string decodeTag(unsigned long encoded) {
        char tag[4];
        tag[0] = (encoded >> 24) & 0xFF;
        tag[1] = (encoded >> 16) & 0xFF;
        tag[2] = (encoded >> 8) & 0xFF;
        tag[3] = encoded & 0xFF;
        return std::string(tag, tag+4);
    }

    bool Font::fontExists(const std::string &family) {
        poFilePath filePath;
        return 	fs::exists(family) || urlForFontFamilyName(family, "", filePath);
    }

    Font *Font::defaultFont() {
        return poGetFont(poFilePath(po::applicationGetResourceDirectory()+"/OpenSans-Regular.ttf"));
    }

    Font::Font()
    :	face(NULL)
    ,	size(0)
    ,	reqStyle("")
    ,	glyph(0)
    ,   loadedGlyph(0)
    ,   currentCache(NULL)
    {
        if(!lib)
            FT_Init_FreeType(&lib);
    }


    //Loads fonts from file
    Font::Font(const poFilePath &filePath, const std::string &style, unsigned long encoding)
    :	face(NULL)
    ,	size(0)
    ,	filePath(filePath)
    ,	reqStyle("")
    ,	glyph(0)
    ,   loadedGlyph(0)
    ,   currentCache(NULL)
    {
        if(!lib)
            FT_Init_FreeType(&lib);

        reqStyle = style;
        
        poFilePath location = filePath;
        
        if(!filePath.exists()) {
            //Try to find the font locally
            //Currently only works on OS X
            poFilePath localFont;
            if(urlForFontFamilyName(filePath.toString(), style, location)) {
                //Its a font family!
                reqFamily = filePath.toString();
            } else {
                return;
            }
        }
        
        FT_Face tmp;
        FT_Error err = FT_New_Face(lib, location.toString().c_str(), 0, &tmp);
        for(int i=1; i<tmp->num_faces; i++) {
            FT_Face f = NULL;
            FT_New_Face(lib, location.toString().c_str(), i, &f);
            
            if(style == f->style_name) {
                FT_Done_Face(tmp);
                tmp = f;
                break;
            }
            else {
                FT_Done_Face(f);
            }
        }
        
        this->face = tmp;
        FT_Select_Charmap(this->face, (FT_Encoding)encoding);

        setPointSize(12);
        setGlyph(0);
    }

    
    Font::~Font() {
        if(face)
            FT_Done_Face(face);
    }
    
    
    //------------------------------------------------------------------
    bool Font::isValid() const {
        return face != NULL;
    }
    
    
    //------------------------------------------------------------------
    std::string Font::getFamilyName() const {
        return face->family_name;
    }
    
    
    //------------------------------------------------------------------
    std::string Font::getStyleName() const {
        return face->style_name;
    }
    
    
    //------------------------------------------------------------------
    poFilePath Font::getFilePath() const {
        return filePath;
    }
    
    
    //------------------------------------------------------------------
    bool Font::hasKerning() const {
        return (face->face_flags & FT_FACE_FLAG_KERNING) != 0;
    }
    
    
    //------------------------------------------------------------------
    std::vector<std::string> Font::getEncodings() const {
        char tag[4];
        std::vector<std::string> encodings;
        for(int i=0; i<face->num_charmaps; i++) {
            FT_CharMap cmap = face->charmaps[i];
            encodings.push_back(decodeTag(cmap->encoding));
        }
        return encodings;
    }
    
    
    //------------------------------------------------------------------
    int Font::getPointSize() const {
        return size;
    }
    
    
    //------------------------------------------------------------------
    void Font::setPointSize(int sz) {
        if(sz != size) {
            size = [sz;
            Point rez = po::deviceResolution();
            FT_Set_Char_Size(face, (size*po::getScale())*64, 0, rez.x, 0);
        }
        
        if ( cachedForSizeYet(size)==false )
            cacheGlyphMetrics();
        
        currentCache = &cachedGlyphMetricsSet[size];
    }
    
    
    //------------------------------------------------------------------
    float Font::getLineHeight() const {
        return (face->size->metrics.height >> 6)/po::getScale();
    }
    
    
    //------------------------------------------------------------------
    float Font::getAscender() const {
        return (face->size->metrics.ascender >> 6)/po::getScale();
    }
    
    
    //------------------------------------------------------------------
    float Font::getDescender() const {
        return (face->size->metrics.descender >> 6)/po::getScale() ;
    }
    
    
    //------------------------------------------------------------------
    float Font::getUnderlinePosition() const {
        return (face->underline_position >> 6)/po::getScale();
    }
    
    
    //------------------------------------------------------------------
    float Font::getUnderlineThickness() const {
        return (face->underline_thickness >> 6)/po::getScale();
    }
    
    
    //------------------------------------------------------------------
    int Font::getGlyph() const {
        return glyph;
    }
    
    
    //------------------------------------------------------------------
    void Font::setGlyph(int g) {
        glyph = g;
    }
    
    
    //------------------------------------------------------------------
    Rect Font::getGlyphBounds() {
        if ( currentCache != NULL && glyph < 128 )
            return (*currentCache)[ glyph ].glyphBounds;
                                    
        loadGlyph( glyph );
        float x = 0;
        float y = 0;
        float w = (face->glyph->metrics.width    >> 6)/po::getScale();
        float h = (face->glyph->metrics.height   >> 6)/po::getScale();
        return Rect(x, y, w, h);
    }
    
    
    //------------------------------------------------------------------
    //Get Glyph Frame & Get Glyph Bounds do not get scaling applied because
    //they use functions that are already scaling
    Rect Font::getGlyphFrame() {
        if ( currentCache != NULL && glyph < 128 )
            return (*currentCache)[ glyph ].glyphFrame;
        
        loadGlyph( glyph );
        return Rect(getGlyphBearing(), getGlyphBounds().getSize());
    }
    
    
    //------------------------------------------------------------------
    float Font::getGlyphDescender() {
        if ( currentCache != NULL && glyph < 128 )
            return (*currentCache)[ glyph ].glyphDescender;
        
        loadGlyph( glyph );
        Rect r = getGlyphFrame();
        return (r.height + r.y);
    }
    
    
    //------------------------------------------------------------------
    Point Font::getGlyphBearing() {
        if ( currentCache != NULL && glyph < 128 )
            return (*currentCache)[ glyph ].glyphBearing;
        
        loadGlyph( glyph );
        float x = (face->glyph->metrics.horiBearingX >> 6)/po::getScale();
        float y = -((face->glyph->metrics.horiBearingY >> 6)/po::getScale());

        return Point(x, y);
    }
    
    
    //------------------------------------------------------------------
    Point Font::getGlyphAdvance() {
        if ( currentCache != NULL && glyph < 128 )
            return (*currentCache)[ glyph ].glyphAdvance;
        
        loadGlyph( glyph );
        float x = (face->glyph->metrics.horiAdvance >> 6)/po::getScale();
        float y = (face->glyph->metrics.vertAdvance >> 6)/po::getScale();
        return Point(x, y);
    }
    
    
    //------------------------------------------------------------------
    poImage* Font::getGlyphImage() {
        currentCache = &cachedGlyphMetricsSet[size];
        
        loadGlyph( glyph );
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        FT_Bitmap bitmap = face->glyph->bitmap;
        
        uint w = bitmap.width + 2;
        uint h = bitmap.rows  + 2;

        ubyte *buffer = new ubyte[w*h]();
        
        for(int i=0; i<bitmap.rows; i++)
            // inset the copy 1 x 1 so the top and left sides doesn't look aliased 
            memcpy(buffer+(i*w)+1, bitmap.buffer+(bitmap.rows-i-1)*bitmap.pitch, bitmap.width);
        
        poImage *img = new poImage(w, h, 1, buffer);
        
        delete [] buffer;
            
        return img;
    }
    
    
    //------------------------------------------------------------------
    //poShape2D *Font::glyphOutline() const {}

    Point Font::kernGlyphs(int glyph1, int glyph2) const {
        if(!hasKerning()) {
            return Point(0,0);
        }
        
        FT_Vector kern;
        FT_Get_Kerning(face, 
                       FT_Get_Char_Index(face, glyph1), FT_Get_Char_Index(face, glyph2),
                       0, &kern);
        return Point(kern.x/po::getScale(), kern.y/po::getScale());
    }
    
    
    //------------------------------------------------------------------
    std::string Font::toString() const {
        return (boost::format("font('%s','%s','%s')")%getFamilyName()%getStyleName()%filePath.toString()).str();
    }
    
    
    //------------------------------------------------------------------
    std::string Font::getRequestedFamilyName() const {
        return reqFamily;
    }
    
    
    //------------------------------------------------------------------
    std::string Font::getRequestedStyleName() const {
        return reqStyle;
    }
    
    
    //------------------------------------------------------------------
    void Font::loadGlyph(int g) {
        if ( loadedGlyph != g ) {
            uint idx = FT_Get_Char_Index(face, g);
            FT_Load_Glyph(face, idx, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT);
            loadedGlyph = g;
        }
    }
    
    
    //------------------------------------------------------------------
    bool Font::cachedForSizeYet(int fontSize) {
        std::map<int,glyphMetricsVector>::iterator iter;
        iter = cachedGlyphMetricsSet.find(fontSize);
        if ( iter == cachedGlyphMetricsSet.end() )
            return false;
        else
            return true;
    }
    
    
    //------------------------------------------------------------------
    void Font::cacheGlyphMetrics() {
        if ( cachedForSizeYet(size) ) {
            currentCache = &cachedGlyphMetricsSet[size];
            return;
        }
        
        glyphMetricsVector tmpVector;
        cachedGlyphMetricsSet[size] = tmpVector;
        cachedGlyphMetricsSet[size].resize(128);
        currentCache = NULL; 
        
        for(int i=0; i<128; i++) {
            FontGlyphMetrics& M = cachedGlyphMetricsSet[size][i];
            setGlyph(i);
            
            M.glyphBounds       = getGlyphBounds();
            M.glyphFrame        = getGlyphFrame();
            M.glyphDescender    = getGlyphDescender();
            M.glyphBearing      = getGlyphBearing();
            M.glyphAdvance      = getGlyphAdvance();
        }

        currentCache = &cachedGlyphMetricsSet[size];
        setGlyph(0);
    }
    
    
    //------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &o, const Font *f) {
        o << f->toString();
        return o;
    }
}/*End po namespace */