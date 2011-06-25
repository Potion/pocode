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

#ifdef _WIN32

#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

bool urlForFontFamilyName(const std::string &family, std::string &response) {
	CFMutableDictionaryRef attributes = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	CFStringRef cfstr = CFStringCreateWithBytes(NULL, (const UInt8*)family.c_str(), family.size(), kCFStringEncodingUTF8, false);
	CFDictionaryAddValue(attributes, kCTFontFamilyNameAttribute, cfstr);
	CFRelease(cfstr);
	
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

bool styleItalic(long flags) {
	return flags & FT_STYLE_FLAG_ITALIC;
}

bool styleBold(long flags) {
	return flags & FT_STYLE_FLAG_BOLD;
}

bool traitMatchesFlags(const std::string &trait, long flags) {
	if(trait == PO_FONT_ITALIC &&  !styleBold(flags) && styleItalic(flags))
		return true;
	if(trait == PO_FONT_BOLD && styleBold(flags) && !styleItalic(flags))
		return true;
	if(trait == PO_FONT_BOLD_ITALIC && styleBold(flags) && styleItalic(flags))
		return true;
	
	return false;
}

FT_Library poFont::lib = NULL;
void deleteFT_Face(FT_Face face) {
	log("done face");
	FT_Done_Face(face);
}

poFont::poFont() 
:	face()
,	size(0)
{}

poFont::poFont(const std::string &family_or_url, int sz, const std::string &trait)
:	face()
,	size(0)
{
	init();
	
	std::string url = "";
	if(fs::exists(family_or_url))
		url = family_or_url;
	else if(!urlForFontFamilyName(family_or_url, url))
		printf("poFont: can't find font (%s)\n", family_or_url.c_str());
	
	FT_Face tmp;
	FT_New_Face(lib, url.c_str(), 0, &tmp);
	for(int i=0; i<tmp->num_faces; i++) {
		FT_Face f = NULL;
		FT_New_Face(lib, url.c_str(), i, &f);
		if(traitMatchesFlags(trait, f->style_flags)) {
			FT_Done_Face(tmp);
			tmp = f;
		}
		else
			FT_Done_Face(f);
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

bool poFont::valid() const {return face;}
poFont *poFont::copy() const {
	poFont *f = new poFont();
	f->face = face;
	f->_url = _url;
	f->_glyph = _glyph;
	f->size = size;
	return f;
}

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

poRect poFont::glyphBounds() {
	float x = 0;
	float y = 0;
	float w = face->glyph->metrics.width >> 6;
	float h = face->glyph->metrics.height >> 6;
	return poRect(x, y, w, h);
}

poPoint poFont::glyphBearing() {
	return poPoint(face->glyph->metrics.horiBearingX >> 6,
				   (face->size->metrics.ascender - face->glyph->metrics.horiBearingY) >> 6);
}

poPoint poFont::glyphAdvance() {
	return poPoint(face->glyph->metrics.horiAdvance >> 6, 
				   face->glyph->metrics.vertAdvance >> 6);
}

poImage *poFont::glyphImage() {
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	
	const FT_Bitmap bitmap = face->glyph->bitmap;
	ubyte *buffer = new ubyte[bitmap.rows * bitmap.width]();
	
	for(int i=0; i<bitmap.rows; i++)
		memcpy(buffer+i*bitmap.width, bitmap.buffer+i*bitmap.pitch, bitmap.width);
	
	poImage *img = new poImage(bitmap.width, bitmap.rows, IMAGE_8, buffer);
	delete [] buffer;
	return img;
}

poShape2D *poFont::glyphOutline() {}

poPoint poFont::kernGlyphs(int glyph1, int glyph2) {
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
	FT_Load_Glyph(face.get(), idx, FT_LOAD_NO_BITMAP);
}

bool fontExists(const std::string &family) {
	std::string url;
	return 	fs::exists(family) || urlForFontFamilyName(family, url);
}

bool operator==(const poFont &a, const poFont &b) {
	return	a.familyName() == b.familyName() &&
			a.styleName() == b.styleName() &&
			a.pointSize() == b.pointSize();
}


poBitmapFontAtlas::poBitmapFontAtlas(poFont *f, int pointSize)
:	poTextureAtlas(GL_ALPHA,512,512)
,	_font(f->copy())
,	size(pointSize)
{
	if(size < 0)
		size = _font->pointSize();
}

poBitmapFontAtlas::~poBitmapFontAtlas() {
	delete _font;
}

void poBitmapFontAtlas::cacheGlyph(uint glyph) {
	if(!hasUID(glyph)) {
		_font->pointSize(size);
		_font->glyph(glyph);
		
		// make sure we clean up the font as we use it
		poResourceStore tmp;
		addImage(tmp.add(_font->glyphImage()),glyph);
		layoutAtlas();
	}
}

poFont const *poBitmapFontAtlas::font() {return _font;}


std::size_t hash_value(const poFont &font) {
	std::size_t seed = 0;
	boost::hash_combine(seed, font.familyName());
	boost::hash_combine(seed, font.styleName());
	boost::hash_combine(seed, font.pointSize());
	return seed;
}

boost::unordered_map<poFont,poBitmapFontAtlas*> BitmapFontCache::atlases;

poBitmapFontAtlas *BitmapFontCache::atlasForFont(poFont *font) {
	if(atlases.find(*font) != atlases.end())
		return atlases[*font];
	
	poBitmapFontAtlas *atlas = new poBitmapFontAtlas(font);
	atlases[*font] = atlas;
	return atlas;
}

void BitmapFontCache::releaseAtlasForFont(poFont *font) {
	boost::unordered_map<poFont,poBitmapFontAtlas*>::iterator iter = atlases.find(*font);
	if(iter != atlases.end()) {
		delete iter->second;
		atlases.erase(iter);
	}
}

std::ostream &operator<<(std::ostream &o, const poFont &f) {
	o << f.toString();
	return o;
}
