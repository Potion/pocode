//
//  poTextLayout.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextLayout.h"
#include "Helpers.h"

#ifdef __APPLE__
#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

struct Range {
	Range(int loc, int len) : location(loc), length(len) {}
	int location, length;
};

struct GlyphCurvePlacement {
	// baseline, middle of box
	poPoint pos;
	float angle;
};

// wow this is bs
CGColorRef cgColor(NSColor *color) {
	NSColor *col = [color colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
	CGFloat colorComponents[4];
	[col getComponents:colorComponents];
	CGColorRef response = (CGColorRef)[(id)CGColorCreateGenericRGB(colorComponents[0],colorComponents[1],colorComponents[2],colorComponents[3]) autorelease];
	return response;
}

CTFontRef ctFont(const std::string &str, const poFontMap &fonts) {
	if(fonts.hasFont(str))
		return (CTFontRef)fonts.font(str)->osFontHandle();
	return ctFont("text", fonts);
}

struct AttributedStringGenerator : public TiXmlVisitor {
	const poFontMap &fonts;
	NSMutableAttributedString *attributed;
	
	AttributedStringGenerator(const poFontMap &fonts, const std::string &str)
	:	fonts(fonts)
	{
		NSString *nsstr = [[NSString alloc] initWithCString:str.c_str() encoding:NSUTF8StringEncoding];
		attributed = [[NSMutableAttributedString alloc] initWithString:nsstr];
		[nsstr release];
	}
	~AttributedStringGenerator() {
		[attributed release];
	}
	virtual bool VisitEnter(const TiXmlElement &ele, const TiXmlAttribute* attribs) {
		Range *r = (Range*)ele.GetUserData();
		NSRange range = NSMakeRange(r->location, r->length);
		
		if(ele.ValueStr() == "text") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("text",fonts) range:range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:range];
		}
		else if(ele.ValueStr() == "i") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("i",fonts) range:range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:range];
		}
		else if(ele.ValueStr() == "b") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("b",fonts) range:range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:range];
		}
		else if(ele.ValueStr() == "u") {
			NSNumber *num = [[NSNumber alloc] initWithUnsignedInt:NSUnderlineStyleSingle];
			[attributed addAttribute:(id)NSUnderlineStyleAttributeName value:(id)num range:range];
			[num release];
		}
		
		return true;
	}
	virtual bool VisitExit(const TiXmlElement &ele) {
		return true;
	}
};

void *createAttributes(TiXmlDocument *doc, const poFontMap &fonts, const std::string &str) {
	AttributedStringGenerator string_gen(fonts, str); 
	doc->Accept(&string_gen);

/*	TODO figure out how best to specify this information
	http://developer.apple.com/library/mac/#documentation/Carbon/Reference/CTParagraphStyleRef/Reference/reference.html
 
	CTTextAlignment align = kCTJustifiedTextAlignment;
	CGFloat indent = 20.0;
	
	// set the paragraph styles here, i guess
	CTParagraphStyleSetting settings[] = {
		{kCTParagraphStyleSpecifierAlignment, sizeof(CTTextAlignment), &align},
		{kCTParagraphStyleSpecifierFirstLineHeadIndent, sizeof(CGFloat), &indent}
	};
	
	CTParagraphStyleRef style = CTParagraphStyleCreate(settings, 2);
	[string_gen.attributed addAttribute:(id)NSParagraphStyleAttributeName value:(id)style range:NSMakeRange(0,string_gen.attributed.length)];
	CFRelease(style);
 */
	
	return [string_gen.attributed copy];
}

void deleteAttributedString(void *attrib) {
	[((NSMutableAttributedString*)attrib) release];
}

void deleteFrame(void *frame) {
	CFRelease((CTFrameRef)frame);
}

void deleteLine(void *line) {
	CFRelease((CTLineRef)line);
}

// will return the CTFrame so you can use it later
// but you have to release the returned CTFrame yourself
void renderTextBox(void *attrib, poRect rect, poTexture **tex, void **ctframe) {
	NSAttributedString *attributed = (NSAttributedString*)attrib;
	
	ubyte *data = new ubyte[(int)ceilf(rect.area())];
	memset(data, 0, rect.area());
	
	CGMutablePathRef path = CGPathCreateMutable();
	CGPathAddRect(path, NULL, CGRectMake(0, 0, rect.width(), rect.height()));
	
	CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)attributed);
	
	CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0,0), path, NULL);
	CFRelease(framesetter);
	CGPathRelease(path);
	
	CGColorSpaceRef space = CGColorSpaceCreateWithName(kCGColorSpaceGenericGray);
	CGContextRef context = CGBitmapContextCreate(data, rect.width(), rect.height(), 8, rect.width(), space, kCGImageAlphaNone);
	CGColorSpaceRelease(space);
	
	CTFrameDraw(frame, context);
	CGContextRelease(context);

	*tex = new poTexture(GL_LUMINANCE, rect.width(), rect.height(), rect.area(), data);
	delete [] data;
	
	*ctframe = (void*)frame;
}

void measureRenderedText(void *ctframe, poRect *actual, poPoint *point) {
	CTFrameRef frame = (CTFrameRef)ctframe;
	
	*actual = poRect(0,0,0,0);
	actual->origin.set(FLT_MAX, FLT_MAX, 0);
	actual->size.set(0, 0, 0);

	CFArrayRef lines = CTFrameGetLines(frame);
	for(CFIndex i=0; i<CFArrayGetCount(lines); i++) {
		CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(lines, i);
		
		CGPoint origin;
		CTFrameGetLineOrigins(frame, CFRangeMake(i,1), &origin);
		
		CGFloat ascent, descent, leading, width, trailing;
		width = CTLineGetTypographicBounds(line, &ascent, &descent, &leading);
		trailing = CTLineGetTrailingWhitespaceWidth(line);
		
		actual->origin.x = MIN(actual->origin.x, origin.x);
		actual->origin.y = MIN(actual->origin.y, origin.y - descent);
		actual->size.x = MAX(actual->size.x, origin.x + width - trailing);
		actual->size.y += ascent + descent + leading;
		
		if(i == 0) {
			*point = poPoint(origin.x, origin.y, 0);
		}
	}	
	
	CGPathRef path = CTFrameGetPath(frame);
	CGRect rect = CGPathGetBoundingBox(path);
	
	actual->origin.y -= (rect.size.height - actual->size.y);
	point->y = rect.size.height - point->y;
}

void layoutTextOnCurve(void *attrib, const std::vector<poPoint> &curve, std::vector<poCurveLayout::Helper> *layout, void **line_out) {
	NSAttributedString *attributed = (NSAttributedString*)attrib;
	
	// make the line object
	CTLineRef line = CTLineCreateWithAttributedString((CFAttributedStringRef)attributed);

	float x = 0.f;
	
	// go thru all the glyphs in the array to make it work
	NSArray *runs_arr = (NSArray*)CTLineGetGlyphRuns(line);
	for(id run_id in runs_arr) {
		CTRunRef run = (CTRunRef)run_id;
		
		CFIndex num_glyphs_run = CTRunGetGlyphCount(run);
		for(CFIndex idx=0; idx<num_glyphs_run; idx++) {
			float ascent, descent, leading;
			float w = CTRunGetTypographicBounds(run, CFRangeMake(idx,1), (CGFloat*) &ascent,(CGFloat*) &descent,(CGFloat*) &leading);

			poPoint origin(0, ascent);
			poPoint size(w, ascent+descent+leading);
			
			poCurveLayout::Helper helper;
			helper.pos = poPoint(x,0.f);
			helper.angle = 0.f;
			helper.bounds = poRect(origin, size);
			layout->push_back(helper);
			
			x += w;
		}
	}
}

void renderTextCurve(void *line_ptr, std::vector<poCurveLayout::Helper> &layout, poTexture **rendered) {
	CTLineRef line = (CTLineRef)line_ptr;

	int count = 0;
	NSArray *runs_arr = (NSArray*)CTLineGetGlyphRuns(line);
	for(id run_id in runs_arr) {
		CTRunRef run = (CTRunRef)run_id;
		
		poCurveLayout::Helper &helper = layout[count];
		
		
		count++;
	}
}


#endif //__APPLE__

// will strip out tags and save the tag's range into the node
struct StripAndIndex : public TiXmlVisitor {
	std::string stripped;
	// these are going to get added to the xml dom 
	// don't forget to clean them up later
	std::vector<Range*> ranges;
	
	virtual bool VisitEnter(const TiXmlElement &ele, const TiXmlAttribute* attribs) {
		TiXmlElement &element = const_cast<TiXmlElement&>(ele);
		ranges.push_back(new Range(utf8strlen(stripped), 0));
		element.SetUserData(ranges.back());
		return true;
	}
	virtual bool VisitExit(const TiXmlElement &ele) {
		Range *range = (Range*)ele.GetUserData();
		range->length = utf8strlen(stripped) - range->location;
		return true;
	}
	virtual bool Visit(const TiXmlText &ele) {
		stripped += ele.ValueStr();
		return true;
	}
};

poTextLayout::poTextLayout()
:	doc(NULL)
,	attrib(NULL)
,	rendered(NULL)
{}

poTextLayout::~poTextLayout() {
	cleanup();
}

void poTextLayout::cleanup() {
	if(doc != NULL) {
		delete doc;
		doc = NULL;
		
		BOOST_FOREACH(void* u, userd) {
			delete ((Range*)u);
		}
	}
	
	if(attrib) {
		deleteAttributedString(attrib);
		attrib = NULL;
	}
	
	if(rendered) {
		delete rendered;
		rendered = NULL;
	}
}

void poTextLayout::stage1() {
	// make sure the string has a "root node" so we can turn it into valid xml
	std::string xml = std::string("<text>") + text + "</text>";
	
	// make sure we leave in any whitespace the user put there on purpose
	bool is_condensed = TiXmlBase::IsWhiteSpaceCondensed();
	TiXmlBase::SetCondenseWhiteSpace(false);
	
	// remove any lingering objects we created
	cleanup();
	
	// make an xml document out of the string we got
	doc = new TiXmlDocument();
	doc->Parse(xml.c_str(), NULL, TIXML_ENCODING_UTF8);
	
	// restore the whitespace setting
	TiXmlBase::SetCondenseWhiteSpace(is_condensed);
	
	// remove tags and make index ranges for where the tags used to be
	// the ranges will be stored as pointers inside the xml document
	StripAndIndex stripper; 
	doc->Accept(&stripper);
	
	// save the stripped string
	stripped = stripper.stripped;
	// save references to the pointers we tucked into the document so we can delete them later
	userd.assign(stripper.ranges.begin(), stripper.ranges.end());

	// make the attributed string 
	attrib = createAttributes(doc, *fonts, stripped);
}

poBoxLayout::poBoxLayout()
:	frame(NULL)
{}

poBoxLayout::~poBoxLayout() {
	cleanup();
}

void poBoxLayout::cleanup() {
	if(frame) {
		deleteFrame(frame);
		frame = NULL;
	}
}

void poBoxLayout::render() {
	cleanup();
	// do all the basics
	poTextLayout::stage1();
	// render the attributed string into 'rendered' and save the frame object for later
	renderTextBox(attrib, bounds, &rendered, &frame);
	// use the frame object to determine the actual text bounds
	measureRenderedText(frame, &actualBounds, &baseline);
}

poCurveLayout::poCurveLayout() {}
poCurveLayout::~poCurveLayout() {}

void poCurveLayout::render() {
	// prep the string, etc
	poTextLayout::stage1();
	
	void *line = NULL;
	layoutTextOnCurve(attrib, curve, &layout, &line);
	renderTextCurve(line, layout, &rendered);
	deleteLine(line);
}




