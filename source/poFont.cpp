//
//  poFont.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poShape2D.h"
#include "Helpers.h"
#include "Loaders.h"
#include "poBitmapFont.h"

#ifdef _WIN32

#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#ifdef POTION_IOS
	#include <CoreText/CoreText.h>
#else
	#include <ApplicationServices/ApplicationServices.h>
#endif

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
	CFRelease(descriptor);
	
	if(!url)
		return false;
	
	char path[1024];
	CFURLGetFileSystemRepresentation(url, true, (UInt8*)path, 1024);
	CFRelease(url);
	
	response = path;
	return true;
}

#endif

FT_Library poFont::lib = NULL;
void deleteFT_Face(FT_Face face) {
	FT_Done_Face(face);
}

poFont::poFont() 
:	face()
,	size(0)
{}

poFont::poFont(const std::string &family_or_url, int sz, const std::string &style)
:	face()
,	size(0)
{
	init();
	
	std::string url = "";
	if(fs::exists(family_or_url))
		url = family_or_url;
	else if(!urlForFontFamilyName(family_or_url, style, url))
		printf("poFont: can't find font (%s)\n", family_or_url.c_str());
	
	FT_Face tmp;
	FT_New_Face(lib, url.c_str(), 0, &tmp);
	for(int i=1; i<tmp->num_faces; i++) {
		FT_Face f = NULL;
		FT_New_Face(lib, url.c_str(), i, &f);
		
		if(style == f->style_name) {
			FT_Done_Face(tmp);
			tmp = f;
		}
		else {
			FT_Done_Face(f);
		}
	}
	
	face = boost::shared_ptr<FT_FaceRec_>(tmp,deleteFT_Face);
	pointSize(sz);
	glyph(0);
}

poFont::~poFont() {}

void poFont::init() {
	if(!lib)
		FT_Init_FreeType(&lib);
}

bool poFont::isValid() const {return face != NULL;}

std::string poFont::familyName() const {return face->family_name;}
std::string poFont::styleName() const {return face->style_name;}
std::string poFont::url() const {return _url;}
bool poFont::hasKerning() const {return (face->face_flags & FT_FACE_FLAG_KERNING) != 0;}

int poFont::pointSize() const {return size;}
void poFont::pointSize(int sz) {
	if(sz != size) {
		size = sz;
		poPoint rez = deviceResolution();
		FT_Set_Char_Size(face.get(), size*64, 0, rez.x, 0);
	}
}

float poFont::lineHeight() const {return face->size->metrics.height >> 6;}
float poFont::ascender() const {return face->size->metrics.ascender >> 6;}
float poFont::descender() const {return face->size->metrics.descender >> 6;}

float poFont::underlinePosition() const {return face->underline_position >> 6;}
float poFont::underlineThickness() const {return face->underline_thickness >> 6;}

int poFont::glyph() const {return _glyph;}
void poFont::glyph(int g) {
	if(g != _glyph) {
		_glyph = g;
		loadGlyph(_glyph);
	}
}

poRect poFont::glyphBounds() const {
	float x = 0;
	float y = 0;
	float w = face->glyph->metrics.width >> 6;
	float h = face->glyph->metrics.height >> 6;
	return poRect(x, y, w, h);
}

poRect poFont::glyphFrame() const {
	return poRect(glyphBearing(), glyphBounds().getSize());
}

float poFont::glyphDescender() const {
	poRect r = glyphFrame();
	return r.height + r.y;
}

poPoint poFont::glyphBearing() const {
	return poPoint(face->glyph->metrics.horiBearingX >> 6,
				   -(face->glyph->metrics.horiBearingY >> 6));
}

poPoint poFont::glyphAdvance() const {
	return poPoint(face->glyph->metrics.horiAdvance >> 6, 
				   face->glyph->metrics.vertAdvance >> 6);
}

poImage *poFont::glyphImage() const {
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

	int padding = GLYPH_PADDING * 2;
	
	const FT_Bitmap bitmap = face->glyph->bitmap;
	ubyte *buffer = new ubyte[(bitmap.rows+padding) * (bitmap.width+padding)]();
	
	for(int i=0; i<bitmap.rows; i++)
		memcpy(buffer+(i+padding/2)*(bitmap.width+padding)+padding/2, bitmap.buffer+i*bitmap.pitch, bitmap.width);
	
	poImageLoader loader;
	poImage *img = loader.load(poImageSpec("", bitmap.width+padding, bitmap.rows+padding, IMAGE_8, buffer));
	
	delete [] buffer;
	return img;
}

poShape2D *poFont::glyphOutline() const {}

poPoint poFont::kernGlyphs(int glyph1, int glyph2) const {
	if(!hasKerning()) {
		return poPoint(0,0);
	}
	
	FT_Vector kern;
	FT_Get_Kerning(face.get(), 
				   FT_Get_Char_Index(face.get(), glyph1), FT_Get_Char_Index(face.get(), glyph2),
				   0, &kern);
	return poPoint(kern.x, kern.y);
}

std::string poFont::toString() const {
	return (boost::format("font: %s %s (%d)")%familyName()%styleName()%pointSize()).str();
}

void poFont::loadGlyph(int g) {
	uint idx = FT_Get_Char_Index(face.get(), g);
	FT_Load_Glyph(face.get(), idx, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT);
}

bool fontExists(const std::string &family) {
	std::string url;
	return 	fs::exists(family) || urlForFontFamilyName(family, "", url);
}

std::ostream &operator<<(std::ostream &o, const poFont &f) {
	o << f.toString();
	return o;
}
