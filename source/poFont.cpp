//
//  poFont.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poShape2D.h"

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

struct poFont::poFontImpl {
	CTFontRef font;
	
	poFontImpl()
	:	font(NULL)
	{}
	
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
	
};
#endif

poFont::poFont()
:	impl(NULL)
{}

poFont::poFont(const std::string &font_name, float point_size) {
	impl = new poFontImpl(font_name, point_size);
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

