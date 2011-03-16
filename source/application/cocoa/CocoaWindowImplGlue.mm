#include <CoreVideo/CoreVideo.h>
#include "CocoaWindowImplGlue.h"
#include "poWindow.h"
#include "poApplication.h"

static BOOL yes_arg = YES;

@interface poOpenGLView : NSOpenGLView {
	NSTimer *timer;
	poWindow *appWindow;
}
@property (assign,nonatomic) poWindow *appWindow;
@end

@implementation poOpenGLView
@synthesize appWindow;

+(NSOpenGLPixelFormat*)defaultPixelFormat {
	NSOpenGLPixelFormatAttribute attributes[] = {
		NSOpenGLPFADoubleBuffer, YES,
		NSOpenGLPFAWindow, YES,
		NSOpenGLPFADepthSize, 32,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFAAccumSize, 64,
		0
	};
	return [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
}

-(BOOL)acceptsFirstResponder {
	return YES;
}

-(BOOL)isFlipped {
	return YES;
}

-(id)initWithFrame:(NSRect)frm context:(NSOpenGLContext*)cxt window:(poWindow*)win{
	self = [super initWithFrame:frm pixelFormat:[poOpenGLView defaultPixelFormat]];
	if(self) {
		self.appWindow = win;
		
		SEL selector = @selector(setNeedsDisplay:);
		NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[poOpenGLView instanceMethodSignatureForSelector:selector]];
		[invocation setTarget:self];
		[invocation setSelector:selector];
		[invocation setArgument:&yes_arg atIndex:2];
		timer = [NSTimer scheduledTimerWithTimeInterval:(1.0/poApplication::get()->framerate)
											 invocation:invocation
												repeats:YES];
		
	}
	return self;
}

-(void)dealloc {
	[timer invalidate];
	[super dealloc];
}

-(void)drawRect:(NSRect)rect {
	self.appWindow->update();
	self.appWindow->draw();
	[self.openGLContext flushBuffer];
}

-(void)keyDown:(NSEvent*)event {
	self.appWindow->keyDown([event.characters characterAtIndex:0], event.keyCode, (int)event.modifierFlags);
}

-(void)keyUp:(NSEvent*)event {
	self.appWindow->keyUp([event.characters characterAtIndex:0], event.keyCode, (int)event.modifierFlags);
}

-(void)mouseDown:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseDown(point.x, point.y, 0);
}

-(void)mouseUp:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseUp(point.x, point.y, 0);
}

-(void)mouseMoved:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseMove(point.x, point.y, 0);
}

-(void)mouseDragged:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseDrag(point.x, point.y, 0);
}
@end


void makeNSWindow(poWindow *powin, void **win, void **gl, int x, int y, int w, int h, poWindowType type) {
	NSScreen *screen = nil;
	NSEnumerator *screens = [[NSScreen screens] objectEnumerator];
	while((screen = [screens nextObject]) && !NSPointInRect(NSMakePoint(x,y), [screen frame])) {}
	if(screen == nil)
		screen = NSScreen.mainScreen;
	
	NSUInteger style_mask;
	switch(type) {
		case WINDOW_TYPE_BORDERLESS:
			style_mask = NSBorderlessWindowMask;
			break;
		case WINDOW_TYPE_FULLSCREEN:
		case WINDOW_TYPE_NORMAL:
			style_mask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
			break;
	}
	
	NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, w, h)
												   styleMask:style_mask
													 backing:NSBackingStoreBuffered
													   defer:NO
													  screen:screen];
	
	NSOpenGLContext *context = nil;
	static NSOpenGLContext *shared_context = nil;
	if(context == nil) {
		shared_context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
													shareContext:nil];
		context = shared_context;
	}
	else {
		context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
											 shareContext:context];
	}
	
	poOpenGLView *opengl = [[poOpenGLView alloc] initWithFrame:NSMakeRect(x,y,w,h)
													   context:context
														window:powin];
	
	window.contentView = opengl;
	[window makeKeyAndOrderFront:nil];
	[window setAcceptsMouseMovedEvents:YES];
	
	*win = window;
	*gl = opengl;
	
	if(type == WINDOW_TYPE_FULLSCREEN)
		powin->setFullscreen(true);
}

void destroyNSWindow(void *win, void *gl) {
	NSWindow *window = (NSWindow*)win;
	[window close];
}

void moveNSWindow(void *win, void *glview, int x, int y, int w, int h) {
	NSWindow *window = (NSWindow*)win;
	[window setFrame:NSMakeRect(x,y,w,h) display:YES];
}

void setNSWindowFullscreen(void *win, void *gl, bool fullscreen) {
	NSWindow *window = (NSWindow*)win;
	NSOpenGLView *glview = (NSOpenGLView*)gl;
	
	if(fullscreen) {
		NSScreen *screen = window.deepestScreen;
		[glview enterFullScreenMode:screen withOptions:[NSDictionary dictionary]];
	}
	else {
		[glview exitFullScreenModeWithOptions:[NSDictionary dictionary]];
	}
}