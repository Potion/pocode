#include "PlatformHelpers.h"
#include <cstdarg>
#include <cstdio>

#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#include <deque>

#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include "poFont.h"
#include "poRect.h"
#include "poTexture.h"

double getTime() {
    static mach_timebase_info_data_t info;
    if(info.denom == 0)
        mach_timebase_info(&info);
    
    uint64_t duration = mach_absolute_time();
    return (duration * info.numer) / (double)info.denom;
}

unsigned int getNumCpus() {
    int count;
    size_t size = sizeof(count);
    if(sysctlbyname("hw.ncpu", &count, &size, NULL, 0))
        return 1;
    return (unsigned int)count;
}

void dprintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

// wow this is bs
CGColorRef cgColor(NSColor *color) {
	NSColor *col = [color colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
	CGFloat colorComponents[4];
	[col getComponents:colorComponents];
	CGColorRef response = (CGColorRef)[(id)CGColorCreateGenericRGB(colorComponents[0],colorComponents[1],colorComponents[2],colorComponents[3]) autorelease];
	return response;
}

CTFontRef ctFont(const std::string &str, const poFontMap &fonts) {
	if(fonts.find(str) != fonts.end())
		return (CTFontRef)fonts.find(str)->second->osFontHandle();
	return ctFont("text", fonts);
}

#include <iostream>

/*	<text>help <em>me <strong>with this</strong></em> bullshit, this sucks</text>
 *	help me with this bullshit, this sucks
 */

// will strip out tags and save the tag's range into the node
struct StripAndIndex : public TiXmlVisitor {
	std::string stripped;
	std::vector<NSRange*> &ranges;

	StripAndIndex(std::vector<NSRange*> &ranges) 
	:	ranges(ranges)
	{}
	
	virtual bool VisitEnter(const TiXmlElement &ele, const TiXmlAttribute* attribs) {
		TiXmlElement &element = const_cast<TiXmlElement&>(ele);
		NSRange *range = new NSRange();
		ranges.push_back(range);
		range->location = utf8strlen(stripped);
		element.SetUserData(range);
		return true;
	}
	virtual bool VisitExit(const TiXmlElement &ele) {
		NSRange *range = (NSRange*)ele.GetUserData();
		range->length = utf8strlen(stripped) - range->location;
		return true;
	}
	virtual bool Visit(const TiXmlText &ele) {
		stripped += ele.ValueStr();
		return true;
	}
};

struct AttributedStringGenerator : public TiXmlVisitor {
	const poFontMap &fonts;
	NSMutableAttributedString *attributed;
	
	AttributedStringGenerator(const poFontMap &fonts, const std::string &str)
	:	fonts(fonts)
	{
		NSString *nsstr = [NSString stringWithCString:str.c_str() encoding:NSUTF8StringEncoding];
		attributed = [[NSMutableAttributedString alloc] initWithString:nsstr];
	}
	~AttributedStringGenerator() {
		[attributed release];
	}
	virtual bool VisitEnter(const TiXmlElement &ele, const TiXmlAttribute* attribs) {
		NSRange *range = (NSRange*)ele.GetUserData();
		
		if(ele.ValueStr() == "text") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("text",fonts) range:*range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:*range];
		}
		else if(ele.ValueStr() == "em") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("em",fonts) range:*range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:*range];
		}
		else if(ele.ValueStr() == "strong") {
			[attributed addAttribute:(id)NSFontAttributeName value:(id)ctFont("strong",fonts) range:*range];
			[attributed addAttribute:(id)NSForegroundColorAttributeName value:(id)cgColor([NSColor whiteColor]) range:*range];
		}
		return true;
	}
	virtual bool VisitExit(const TiXmlElement &ele) {
		return true;
	}
};

poTexture *renderTextWithAttributedString(poRect rect, NSAttributedString *attributed, poRect *actual) {
	*actual = poRect(0,0,0,0);

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

	CFArrayRef lines = CTFrameGetLines(frame);
	for(CFIndex i=0; i<CFArrayGetCount(lines); i++) {
		CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(lines, i);
		
		CGPoint origin;
		CTFrameGetLineOrigins(frame, CFRangeMake(i,1), &origin);
		
		CGFloat ascent, descent, leading, width;
		width = CTLineGetTypographicBounds(line, &ascent, &descent, &leading);
		
		NSLog(@"%@", NSStringFromPoint(origin));
		
		actual->top = origin.y;
		actual->right = MAX(actual->right, width);
	}
	
	CFRelease(frame);
	
	poTexture *tex = new poTexture(GL_LUMINANCE, rect.width(), rect.height(), rect.area(), data);
	delete [] data;
	
	return tex;
}

void renderTextToTexture(poRect rect, TiXmlDocument *doc, const poFontMap &fonts, poTexture **tex, std::string *stripped, poRect *actual) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	std::vector<NSRange*> ranges;
	StripAndIndex stripper(ranges); 
	doc->Accept(&stripper);
	
	AttributedStringGenerator string_gen(fonts, stripper.stripped); 
	doc->Accept(&string_gen);

	poTexture *texture = renderTextWithAttributedString(rect, string_gen.attributed, actual);
	
	// make sure to clean up the user data
	BOOST_FOREACH(NSRange* range, ranges) {
		delete range;
	}
	
	*tex = texture;
	*stripped = stripper.stripped;
	
	[pool release];
}

int utf8strlen(const std::string &str) {
	int i=0, j=0;
	while (str[i]) {
		if ((str[i] & 0xC0) != 0x80)
			j++;
		i++;
	}
	return j;
}

