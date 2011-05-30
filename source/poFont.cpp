//
//  poFont.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poShape2D.h"

const std::string poFontMap::REGULAR_FONT_KEY = "text";
const std::string poFontMap::ITALIC_FONT_KEY = "i";
const std::string poFontMap::BOLD_FONT_KEY = "b";

#ifdef _WIN32

#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

struct PathApplicatorHelper {
	poShape2D *shape;
	CGRect bounds;
};

static void pathApplicator(void *info, const CGPathElement *element) {
	PathApplicatorHelper *helper = (PathApplicatorHelper*)info;
	
	switch(element->type) {
		case kCGPathElementMoveToPoint:
		case kCGPathElementAddLineToPoint: 
		{
			poPoint pt(element->points[0].x, helper->bounds.size.height-element->points[0].y, 0);
			helper->shape->addPoint(pt);
			break;
		}
		case kCGPathElementAddQuadCurveToPoint:
		{
			poPoint control(element->points[0].x, helper->bounds.size.height-element->points[0].y, 0);
			poPoint coord(element->points[1].x, helper->bounds.size.height-element->points[1].y, 0);
			helper->shape->curveTo(coord, control);
			break;
		}
		case kCGPathElementAddCurveToPoint: 
		{
			poPoint control1(element->points[0].x, helper->bounds.size.height-element->points[0].y, 0);
			poPoint control2(element->points[1].x, helper->bounds.size.height-element->points[1].y, 0);
			poPoint coord(element->points[2].x, helper->bounds.size.height-element->points[2].y, 0);
			helper->shape->curveTo(coord, control1, control2);
			break;
		}
		case kCGPathElementCloseSubpath:
		{
			break;
		}
	}
}

static CTFontSymbolicTraits appleFontTraits(int traits) {
	CTFontSymbolicTraits apple_traits = 0;
	if(traits & FONT_ITALIC)
		apple_traits |= kCTFontItalicTrait;
	if(traits & FONT_BOLD)
		apple_traits |= kCTFontBoldTrait;
	if(traits & FONT_EXPANDED)
		apple_traits |= kCTFontExpandedTrait;
	if(traits & FONT_CONDENSED)
		apple_traits |= kCTFontCondensedTrait;
	if(traits & FONT_MONO)
		apple_traits |= kCTFontMonoSpaceTrait;
	return apple_traits;
}

struct poFont::poFontImpl {
	CTFontRef font;
	
	poFontImpl()
	:	font(NULL)
	{}
	
	poFontImpl(const std::string &family, int traits, float point_size) {
		CFMutableDictionaryRef attributes = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		
		CFStringRef cfstr = CFStringCreateWithBytes(NULL, (const UInt8*)family.c_str(), family.size(), kCFStringEncodingUTF8, false);
		CFDictionaryAddValue(attributes, kCTFontFamilyNameAttribute, cfstr);
		CFRelease(cfstr);
		
		CTFontSymbolicTraits apple_traits = appleFontTraits(traits);
		CFNumberRef apple_traits_num = CFNumberCreate(NULL, kCFNumberSInt32Type, &apple_traits);
		
		CFMutableDictionaryRef traits_dict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFDictionaryAddValue(traits_dict, kCTFontSymbolicTrait, apple_traits_num);
		CFDictionaryAddValue(attributes, kCTFontTraitsAttribute, traits_dict);
		
		CFRelease(traits_dict);
		CFRelease(apple_traits_num);
		
		CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes(attributes);
		CFRelease(attributes);
		
		font = CTFontCreateWithFontDescriptor(descriptor, point_size, NULL);
		CFRelease(descriptor);
	}
	
	poFontImpl(const std::string &name, float size) {
		CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8*)name.c_str(), name.size(), true);
		CFArrayRef descriptors = CTFontManagerCreateFontDescriptorsFromURL(url);

		CTFontDescriptorRef descriptor = (CTFontDescriptorRef)CFArrayGetValueAtIndex(descriptors, 0);
		CFRetain(descriptor);
		
		font = CTFontCreateWithFontDescriptor(descriptor, size, NULL);

		CFRelease(url);
		CFRelease(descriptors);
	}
	
	poFontImpl *copy() {
		poFontImpl *impl = new poFontImpl();
		CTFontDescriptorRef desc = CTFontCopyFontDescriptor(font);
		impl->font = CTFontCreateCopyWithAttributes(font, CTFontGetSize(font), NULL, desc);
		CFRelease(desc);
		return impl;
	}
	
	~poFontImpl() {
		CFRelease(font);
	}
	
	std::string name() const {
		CFStringRef cfstr = CTFontCopyFullName(font);
		CFIndex len = CFStringGetLength(cfstr);
		
		unsigned char *buffer = new unsigned char[len];
		CFStringGetBytes(cfstr, CFRangeMake(0, len), kCFStringEncodingUTF8, '?', false, buffer, len, NULL);
		std::string response((char*)buffer, len);
		delete [] buffer;

		CFRelease(cfstr);
		return response;
	}
	
	float size() const {
		return CTFontGetSize(font);
	}
	
	poShape2D *getGlyphOutline(const std::string &str) {
		CFStringRef cfstr = CFStringCreateWithBytes(NULL, (const UInt8*)str.c_str(), str.size(), kCFStringEncodingUTF8, false);
		CFIndex length = CFStringGetLength(cfstr);
		CGGlyph *glyphs = new CGGlyph[length];
		CTFontGetGlyphsForCharacters(font, CFStringGetCharactersPtr(cfstr), glyphs, length);
		
		CGPathRef path = CTFontCreatePathForGlyph(font, glyphs[0], NULL);
		
		PathApplicatorHelper helper;
		helper.shape = new poShape2D();
		helper.bounds = CGPathGetBoundingBox(path);
		CGPathApply(path, &helper, pathApplicator);
		
		delete [] glyphs;
		CGPathRelease(path);
		CFRelease(cfstr);
		return helper.shape;
	}

	void *osFontHandle() {
		return (void*)font;
	}
};
#endif

poFont::poFont()
:	impl(NULL)
{}

poFont::poFont(const std::string &family, int traits, float point_size) {
	impl = new poFontImpl(family, traits, point_size);
}

poFont::poFont(const std::string &url, float point_size) {
	impl = new poFontImpl(url, point_size);
}

poFont::~poFont() {
	delete impl;
}

poFont *poFont::copy() {
	poFont *font = new poFont();
	font->impl = impl->copy();
	return font;
}

std::string poFont::name() const {return impl->name();}
float poFont::size() const {return impl->size();}

poShape2D *poFont::getGlyphOutline(const std::string &str) {
	return impl->getGlyphOutline(str);
}

void *poFont::osFontHandle() const {
	return impl->osFontHandle();
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
	fonts[name] = resources.add(font->copy());
	return *this;
	
}
