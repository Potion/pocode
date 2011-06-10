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

const std::string poFontMap::REGULAR_FONT_KEY = "text";
const std::string poFontMap::ITALIC_FONT_KEY = "i";
const std::string poFontMap::BOLD_FONT_KEY = "b";

#ifdef _WIN32

#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

std::string urlForFontFamilyName(const std::string &family) {
	CFMutableDictionaryRef attributes = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	CFStringRef cfstr = CFStringCreateWithBytes(NULL, (const UInt8*)family.c_str(), family.size(), kCFStringEncodingUTF8, false);
	CFDictionaryAddValue(attributes, kCTFontFamilyNameAttribute, cfstr);
	CFRelease(cfstr);
	
	CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes(attributes);
	CFRelease(attributes);
	
	CFURLRef url = (CFURLRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontURLAttribute);
	CFRelease(descriptor);
	
	if(!url)
		return "";
	
	char path[1024];
	CFURLGetFileSystemRepresentation(url, true, (UInt8*)path, 1024);
	CFRelease(url);
	
	return path;
}

#endif

bool styleItalic(long flags) {
	return flags & FT_STYLE_FLAG_ITALIC;
}

bool styleBold(long flags) {
	return flags & FT_STYLE_FLAG_BOLD;
}

bool traitsMatchFlags(poFontTraits traits, long flags) {
	if((traits & PO_FONT_ITALIC) && (traits & PO_FONT_BOLD) && styleBold(flags) && styleItalic(flags))
		return true;
	else if((traits & PO_FONT_ITALIC) && styleItalic(flags) && !styleBold(flags))
		return true;
	else if((traits & PO_FONT_BOLD) && styleBold(flags) && !styleItalic(flags))
		return true;
	return false;
}

FT_Library poFont::lib = NULL;
void deleteFT_Face(FT_Face face) {FT_Done_Face(face);}

poFont::poFont() 
:	face()
,	size(0)
{}

poFont::poFont(const std::string &family, poFontTraits traits, int sz)
:	face()
,	size(0)
{
	FT_Face tmp;
	
	init();
	std::string url = urlForFontFamilyName(family);
	FT_New_Face(lib, url.c_str(), 0, &tmp);
	if(face && traits != 0) {
		for(int i=0; i<face->num_faces; i++) {
			FT_Face f = NULL;
			FT_New_Face(lib, url.c_str(), i, &f);
			if(traitsMatchFlags(traits, f->style_flags)) {
				FT_Done_Face(tmp);
				tmp = f;
			}
			else
				FT_Done_Face(f);
		}
	}
	
	face = boost::shared_ptr<FT_FaceRec_>(tmp,deleteFT_Face);
	pointSize(sz);
	glyph(0);
}

poFont::poFont(const std::string &url, int sz)
:	face()
,	size(0)
{
	init();
	FT_Face tmp;
	FT_New_Face(lib, url.c_str(), 0, &tmp);
	face = boost::shared_ptr<FT_FaceRec_>(tmp,deleteFT_Face);
	
	pointSize(sz);
}

poFont::~poFont() {}

void poFont::init() {
	if(!lib)
		FT_Init_FreeType(&lib);
}

bool poFont::valid() const {return face;}

std::string poFont::familyName() const {return face->family_name;}
std::string poFont::styleName() const {return face->style_name;}
std::string poFont::url() const {return _url;}

int poFont::pointSize() const {return size;}
void poFont::pointSize(int sz) {
	if(sz != size) {
		size = sz;
		poPoint rez = deviceResolution();
		FT_Set_Char_Size(face.get(), size*64, size*64, rez.x, rez.y);
	}
}

float poFont::lineHeight() const {return face->height >> 6;}
float poFont::underlinePosition() const {return face->underline_position >> 6;}
float poFont::underlineThickness() const {return face->underline_thickness >> 6;}

int poFont::glyph() const {return _glyph;}
void poFont::glyph(int g) {
	_glyph = g;
	loadGlyph(_glyph);
}

poRect poFont::glyphBounds() {
	float x = 0;
	float y = 0;
	float w = face->glyph->metrics.width >> 6;
	float h = face->glyph->metrics.height >> 6;
	return poRect(x, y, w, h);
}

poPoint poFont::glyphBearing() {
	return poPoint((face->glyph->metrics.horiBearingX >> 6),
				   (face->glyph->metrics.horiBearingY >> 6));
}

poPoint poFont::glyphAdvance() {
	return poPoint((face->glyph->metrics.horiAdvance >> 6), 
				   (face->glyph->metrics.vertAdvance >> 6));
}

poImage *poFont::glyphImage() {
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	
	const FT_Bitmap bitmap = face->glyph->bitmap;
	ubyte *buffer = new ubyte[bitmap.rows * bitmap.width]();
	
	for(int i=0; i<bitmap.rows; i++)
		memcpy(buffer+i*bitmap.width, bitmap.buffer+i*bitmap.pitch, bitmap.width);
	
	return new poImage(bitmap.width, bitmap.rows, IMAGE_8, buffer);
}

poShape2D *poFont::glyphOutline() {}

poPoint poFont::kernGlyphs(int glyph1, int glyph2) {
	FT_Vector kern;
	FT_Get_Kerning(face.get(), 
				   FT_Get_Char_Index(face.get(), glyph1), FT_Get_Char_Index(face.get(), glyph2),
				   0, &kern);
	return poPoint(kern.x, kern.y);
}

poTextureAtlas *poFont::atlasForFont(int glyph_start, int glyph_end) {
	poTextureAtlas *atlas = new poTextureAtlas(GL_ALPHA, 1024, 1024);
	for(int i=glyph_start; i<=glyph_end; i++) {
		glyph(i);
		atlas->addImage(glyphImage(), i);
	}
	atlas->layoutAtlas();
	return atlas;
}

std::string poFont::toString() const {
	return (boost::format("font: %s %s (%d)")%familyName()%styleName()%pointSize()).str();
}

void poFont::loadGlyph(int g) {
	uint idx = FT_Get_Char_Index(face.get(), g);
	FT_Load_Glyph(face.get(), idx, FT_LOAD_NO_BITMAP);
}



bool poFontMap::hasFont(const std::string &name) const {
	if(fonts.find(name) != fonts.end())
		return true;
	return false;
}

poFont *poFontMap::font(const std::string &name) const {
	if(hasFont(name))
		return fonts.find(name)->second;
	return NULL;
}

poFontMap &poFontMap::font(const std::string &name, poFont *font) {
	if(hasFont(name))
		delete resources.remove(fonts[name]);
	fonts[name] = resources.add(new poFont(*font));
	return *this;
}


std::ostream &operator<<(std::ostream &o, const poFont &f) {
	o << f.toString();
	return o;
}
