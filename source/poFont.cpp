//
//  poFont.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poShape2D.h"
#include "poHelpers.h"

#ifdef _WIN32

#elif defined(__APPLE__)
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

FT_Library poFont::FontImpl::lib = NULL;

poFont::FontImpl::FontImpl()
:	face(NULL)
,	size(0)
,	url("")
,	glyph(0)
{
	if(!lib)
		FT_Init_FreeType(&lib);
}

poFont::FontImpl::FontImpl(const std::string &family_or_url, const std::string &style) 
:	face(NULL)
,	size(0)
,	url("")
,	glyph(0)
{
	if(!lib)
		FT_Init_FreeType(&lib);

	std::string url = "";
	if(fs::exists(family_or_url))
		url = family_or_url;
	else if(!urlForFontFamilyName(family_or_url, style, url)) {
		printf("poFont: can't find font (%s)\n", family_or_url.c_str());
		return;
	}
	
	FT_Face tmp;
	FT_New_Face(lib, url.c_str(), 0, &tmp);
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
}

poFont::FontImpl::~FontImpl() {
	if(face)
		FT_Done_Face(face);
}

bool poFont::fontExists(const std::string &family) {
	std::string url;
	return 	fs::exists(family) || urlForFontFamilyName(family, "", url);
}

poFont::poFont() 
{}

poFont::poFont(const std::string &family_or_url, const std::string &style) {
	shared.reset(new FontImpl(family_or_url, style));
	setPointSize(12);
	setGlyph(0);
}

poFont::~poFont() {
}

bool poFont::isValid() const {
	return shared && shared->face != NULL;
}

std::string poFont::getFamilyName() const {
	return shared->face->family_name;
}
std::string poFont::getStyleName() const {
	return shared->face->style_name;
}
std::string poFont::getUrl() const {
	return shared->url;
}
bool poFont::hasKerning() const {
	return (shared->face->face_flags & FT_FACE_FLAG_KERNING) != 0;
}

int poFont::getPointSize() const {
	return shared->size;
}

void poFont::setPointSize(int sz) {
	if(sz != shared->size) {
		shared->size = sz;
		poPoint rez = deviceResolution();
		FT_Set_Char_Size(shared->face, shared->size*64, 0, rez.x, 0);
	}
}

float poFont::getLineHeight() const {
	return shared->face->size->metrics.height >> 6;
}
float poFont::getAscender() const {
	return shared->face->size->metrics.ascender >> 6;
}
float poFont::getDescender() const {
	return shared->face->size->metrics.descender >> 6;
}

float poFont::getUnderlinePosition() const {
	return shared->face->underline_position >> 6;
}
float poFont::getUnderlineThickness() const {
	return shared->face->underline_thickness >> 6;
}

int poFont::getGlyph() const {
	return shared->glyph;
}

void poFont::setGlyph(int g) {
	if(g != shared->glyph) {
		shared->glyph = g;
		loadGlyph(shared->glyph);
	}
}

poRect poFont::getGlyphBounds() const {
	float x = 0;
	float y = 0;
	float w = shared->face->glyph->metrics.width >> 6;
	float h = shared->face->glyph->metrics.height >> 6;
	return poRect(x, y, w, h);
}

poRect poFont::getGlyphFrame() const {
	return poRect(getGlyphBearing(), getGlyphBounds().getSize());
}

float poFont::getGlyphDescender() const {
	poRect r = getGlyphFrame();
	return r.height + r.y;
}

poPoint poFont::getGlyphBearing() const {
	return poPoint(shared->face->glyph->metrics.horiBearingX >> 6,
				   -(shared->face->glyph->metrics.horiBearingY >> 6));
}

poPoint poFont::getGlyphAdvance() const {
	return poPoint(shared->face->glyph->metrics.horiAdvance >> 6, 
				   shared->face->glyph->metrics.vertAdvance >> 6);
}

poImage poFont::getGlyphImage() const {
	FT_Render_Glyph(shared->face->glyph, FT_RENDER_MODE_NORMAL);
	const FT_Bitmap bitmap = shared->face->glyph->bitmap;
	
	uint w = bitmap.width+1;
	uint h = bitmap.rows+1;

	ubyte *buffer = new ubyte[w*h]();
	
	for(int i=0; i<bitmap.rows; i++)
		// inset the copy 1 x 1 so the top and left sides doesn't look aliased 
		memcpy(buffer+(i*w)+1, bitmap.buffer+(bitmap.rows-i-1)*bitmap.pitch, bitmap.width);
	
	poImage img(w, h, 1, buffer);
	
	delete [] buffer;
	return img;
}

//poShape2D *poFont::glyphOutline() const {}

poPoint poFont::kernGlyphs(int glyph1, int glyph2) const {
	if(!hasKerning()) {
		return poPoint(0,0);
	}
	
	FT_Vector kern;
	FT_Get_Kerning(shared->face, 
				   FT_Get_Char_Index(shared->face, glyph1), FT_Get_Char_Index(shared->face, glyph2),
				   0, &kern);
	return poPoint(kern.x, kern.y);
}

std::string poFont::toString() const {
	return (boost::format("font('%s','%s','%s')")%getFamilyName()%getStyleName()%shared->url).str();
}

void poFont::loadGlyph(int g) {
	uint idx = FT_Get_Char_Index(shared->face, g);
	FT_Load_Glyph(shared->face, idx, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT);
}

std::ostream &operator<<(std::ostream &o, const poFont *f) {
	o << f->toString();
	return o;
}



