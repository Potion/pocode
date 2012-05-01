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
//  poFont.cpp
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
using namespace std;

#ifdef POTION_WIN32

	#include "windows.h"

		bool urlForFontFamilyName(const std::string &family, const std::string &style, std::string &response) {
			return false;
		}

#elif defined(POTION_APPLE)
	#include <CoreFoundation/CoreFoundation.h>

	#if defined(POTION_MAC)
		#include <ApplicationServices/ApplicationServices.h>

		bool urlForFontFamilyName(const std::string &family, const std::string &style, std::string &response) {
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
			
			response = (char*)path;
			return true;
		}

	#else

		bool urlForFontFamilyName(const std::string &family, const std::string &style, std::string &response) {
			return false;
		}

	#endif

#endif
FT_Library poFont::lib = NULL;

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

bool poFont::fontExists(const std::string &family) {
	std::string url;
	return 	fs::exists(family) || urlForFontFamilyName(family, "", url);
}

poFont *poFont::defaultFont() {
	return poGetFont(applicationGetResourceDirectory()+"/OpenSans-Regular.ttf");
}

poFont::poFont()
:	face(NULL)
,	size(0)
,	url("")
,	reqUrlOrFamily("")
,	reqStyle("")
,	glyph(0)
,   loadedGlyph(0)
,   currentCache(NULL)
{
	if(!lib)
		FT_Init_FreeType(&lib);
}

poFont::poFont(const std::string &family_or_url, const std::string &style, unsigned long encoding)
:	face(NULL)
,	size(0)
,	url("")
,	reqUrlOrFamily("")
,	reqStyle("")
,	glyph(0)
,   loadedGlyph(0)
,   currentCache(NULL)
{
	if(!lib)
		FT_Init_FreeType(&lib);

	reqUrlOrFamily = family_or_url;
	reqStyle = style;
	
	if(fs::exists(family_or_url))
		url = family_or_url;
	else if(!urlForFontFamilyName(family_or_url, style, url))
        return;
	
	FT_Face tmp;
	FT_Error err = FT_New_Face(lib, url.c_str(), 0, &tmp);
	for(int i=1; i<tmp->num_faces; i++) {
		FT_Face f = NULL;
		FT_New_Face(lib, url.c_str(), i, &f);
		
		if(style == f->style_name) {
			FT_Done_Face(tmp);
			tmp = f;
			break;
		}
		else {
			FT_Done_Face(f);
		}
	}
	
	this->url = url;
	this->face = tmp;
	FT_Select_Charmap(this->face, (FT_Encoding)encoding);

	setPointSize(12);
	setGlyph(0);
}

poFont::~poFont() {
	if(face)
		FT_Done_Face(face);
}

bool poFont::isValid() const {
	return face != NULL;
}

std::string poFont::getFamilyName() const {
	return face->family_name;
}
std::string poFont::getStyleName() const {
	return face->style_name;
}
std::string poFont::getUrl() const {
	return url;
}
bool poFont::hasKerning() const {
	return (face->face_flags & FT_FACE_FLAG_KERNING) != 0;
}

std::vector<std::string> poFont::getEncodings() const {
	char tag[4];
	std::vector<std::string> encodings;
	for(int i=0; i<face->num_charmaps; i++) {
		FT_CharMap cmap = face->charmaps[i];
		encodings.push_back(decodeTag(cmap->encoding));
	}
	return encodings;
}

int poFont::getPointSize() const {
	return size;
}

void poFont::setPointSize(int sz) {
	if(sz != size) {
		size = sz;
		poPoint rez = deviceResolution();
		FT_Set_Char_Size(face, (size*poGetScale())*64, 0, rez.x, 0);
	}
    
    if ( cachedForSizeYet(size)==false )
        cacheGlyphMetrics();
    
    currentCache = &cachedGlyphMetricsSet[size];
}

float poFont::getLineHeight() const {
	return (face->size->metrics.height >> 6)/poGetScale();
}
float poFont::getAscender() const {
	return ceil((face->size->metrics.ascender >> 6)/poGetScale());
}
float poFont::getDescender() const {
	return floor((face->size->metrics.descender >> 6)/poGetScale());
}

float poFont::getUnderlinePosition() const {
	return (face->underline_position >> 6)/poGetScale();
}
float poFont::getUnderlineThickness() const {
	return (face->underline_thickness >> 6)/poGetScale();
}

int poFont::getGlyph() const {
	return glyph;
}

void poFont::setGlyph(int g) {
    glyph = g;
}

poRect poFont::getGlyphBounds() {
    if ( currentCache != NULL && glyph < 128 )
        return (*currentCache)[ glyph ].glyphBounds;
                                
    loadGlyph( glyph );
	float x = 0;
	float y = 0;
	float w = (face->glyph->metrics.width    >> 6)/poGetScale();
	float h = (face->glyph->metrics.height   >> 6)/poGetScale();
	return poRect(x, y, w, h);
}


//Get Glyph Frame & Get Glyph Bounds do not get scaling applied because
//they use functions that are already scaling
poRect poFont::getGlyphFrame() {
    if ( currentCache != NULL && glyph < 128 )
        return (*currentCache)[ glyph ].glyphFrame;
    
    loadGlyph( glyph );
	return poRect(getGlyphBearing(), getGlyphBounds().getSize());
}

float poFont::getGlyphDescender() {
    if ( currentCache != NULL && glyph < 128 )
        return (*currentCache)[ glyph ].glyphDescender;
    
    loadGlyph( glyph );
	poRect r = getGlyphFrame();
	return (r.height + r.y);
}

poPoint poFont::getGlyphBearing()  {
    if ( currentCache != NULL && glyph < 128 )
        return (*currentCache)[ glyph ].glyphBearing;
    
    loadGlyph( glyph );
    int x = (face->glyph->metrics.horiBearingX >> 6)/poGetScale();
    int y = -(face->glyph->metrics.horiBearingY >> 6)/poGetScale();
	return poPoint(x, y);
}

poPoint poFont::getGlyphAdvance() {
    if ( currentCache != NULL && glyph < 128 )
        return (*currentCache)[ glyph ].glyphAdvance;
    
    loadGlyph( glyph );
	return poPoint((face->glyph->metrics.horiAdvance >> 6)/poGetScale(), 
				   (face->glyph->metrics.vertAdvance >> 6)/poGetScale());
}

poImage* poFont::getGlyphImage() {
    currentCache = &cachedGlyphMetricsSet[size];
    
    loadGlyph( glyph );
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	const FT_Bitmap bitmap = face->glyph->bitmap;
	
	uint w = bitmap.width+1;
	uint h = bitmap.rows+1;

	ubyte *buffer = new ubyte[w*h]();
	
	for(int i=0; i<bitmap.rows; i++)
		// inset the copy 1 x 1 so the top and left sides doesn't look aliased 
		memcpy(buffer+(i*w)+1, bitmap.buffer+(bitmap.rows-i-1)*bitmap.pitch, bitmap.width);
	
	poImage *img = new poImage(w, h, 1, buffer);
	
	delete [] buffer;
        
	return img;
}

//poShape2D *poFont::glyphOutline() const {}

poPoint poFont::kernGlyphs(int glyph1, int glyph2) const {
	if(!hasKerning()) {
		return poPoint(0,0);
	}
	
	FT_Vector kern;
	FT_Get_Kerning(face, 
				   FT_Get_Char_Index(face, glyph1), FT_Get_Char_Index(face, glyph2),
				   0, &kern);
	return poPoint(kern.x/poGetScale(), kern.y/poGetScale());
}

std::string poFont::toString() const {
	return (boost::format("font('%s','%s','%s')")%getFamilyName()%getStyleName()%url).str();
}

std::string poFont::getRequestedFamilyName() const {
	return reqUrlOrFamily;
}

std::string poFont::getRequestedStyleName() const {
	return reqStyle;
}

void poFont::loadGlyph(int g) {
    if ( loadedGlyph != g )
    {
        uint idx = FT_Get_Char_Index(face, g);
        FT_Load_Glyph(face, idx, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT);
        loadedGlyph = g;
    }
}

bool    poFont::cachedForSizeYet(int fontSize)
{
    std::map<int,glyphMetricsVector>::iterator iter;
    iter = cachedGlyphMetricsSet.find(fontSize);
    if ( iter == cachedGlyphMetricsSet.end() )
        return false;
    else
        return true;
}


void    poFont::cacheGlyphMetrics()
{
    if ( cachedForSizeYet(size) )
    {
        currentCache = &cachedGlyphMetricsSet[size];
        return;
    }
    
    glyphMetricsVector tmpVector;
    cachedGlyphMetricsSet[size] = tmpVector;
    cachedGlyphMetricsSet[size].resize(128);
    currentCache = NULL; 
    
    for( int i=0; i<128; i++ )
    {
        poFontGlyphMetrics& M = cachedGlyphMetricsSet[size][i];
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

std::ostream &operator<<(std::ostream &o, const poFont *f) {
	o << f->toString();
	return o;
}



