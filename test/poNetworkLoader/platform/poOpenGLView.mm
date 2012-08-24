//
//  poOpenGLView.m
//  pocode
//
//  Created by Joshua Fisher on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import "poOpenGLView.h"
#import "AppDelegate.h"
#include "poWindow.h"

static NSLock *lock = [[NSLock alloc] init];

CVReturn MyDisplayLinkCallback (CVDisplayLinkRef displayLink,
								const CVTimeStamp *inNow,
								const CVTimeStamp *inOutputTime,
								CVOptionFlags flagsIn,
								CVOptionFlags *flagsOut,
								void *displayLinkContext)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	poOpenGLView *self = (poOpenGLView*)displayLinkContext;

	[lock lock];
	if([self canDraw]) {
		NSOpenGLContext *context = [self openGLContext];
		CGLContextObj cglcontext = (CGLContextObj)[context CGLContextObj];

		CGLLockContext(cglcontext);
		[context makeCurrentContext];
		
		self.appWindow->makeCurrent();
		self.appWindow->update();
		self.appWindow->draw();

		CGLUnlockContext(cglcontext);

		[context flushBuffer];
	}
	[lock unlock];

	[pool release];
	return kCVReturnSuccess;
}

@implementation poOpenGLView

@synthesize appWindow, openGLContext=context;

+(NSOpenGLPixelFormat*)defaultPixelFormat {
	NSOpenGLPixelFormatAttribute attributes[] = {
		NSOpenGLPFADoubleBuffer, YES,
		NSOpenGLPFAWindow, YES,
		NSOpenGLPFADepthSize, 32,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFAAccumSize, 64,
		NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
		0
	};
	return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(id)initWithFrame:(NSRect)frm {
	self = [super initWithFrame:frm];
	if(self) {
		display_link = nil;
		
//		[[NSNotificationCenter defaultCenter] addObserverForName:NSViewGlobalFrameDidChangeNotification
//														  object:self
//														   queue:nil
//													  usingBlock:^(NSNotification* notice) {
//														  [self update];
//													  }];
//		
	}
	return self;
}

-(void)dealloc {
//	[[NSNotificationCenter defaultCenter] removeObserver:self
//													name:NSViewGlobalFrameDidChangeNotification
//												  object:self];
	
	[self stopAnimating];
	
	if(self.appWindow) {
		self.appWindow = NULL;
	}
	
	self.openGLContext = nil;
	
	[super dealloc];
}

-(BOOL)acceptsFirstResponder {
	return YES;
}

-(BOOL)isFlipped {
	return YES;
}

-(void)update {
	NSRect rect = [self.window contentRectForFrameRect:self.window.frame];
	rect.origin = self.window.frame.origin;
	self.appWindow->resized(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	[context update];
}

-(void)lockFocus {
	[super lockFocus];
	if(context.view != self)
		[context setView:self];
	[context makeCurrentContext];
}

-(void)viewDidMoveToWindow {
	[super viewDidMoveToWindow];
	
	NSWindow *window = self.window;
	if(window) {
		[[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidResizeNotification
														  object:window
														   queue:nil
													  usingBlock:^(NSNotification* notice) {
														  [self update];
													  }];
		[self startAnimating];
	}
	else {
		[[NSNotificationCenter defaultCenter] removeObserver:self
														name:NSWindowDidResizeNotification
													  object:nil];

		[self stopAnimating];
	}
}

-(BOOL)isAnimating {
	return display_link != nil;
}

-(void)startAnimating {
	if(!display_link) {
		CGDirectDisplayID displayID = (CGDirectDisplayID)[[[[self.window screen] deviceDescription] objectForKey:@"NSScreenNumber"] intValue];
		CVDisplayLinkCreateWithCGDisplay(displayID, &display_link);
		CVDisplayLinkSetOutputCallback(display_link, MyDisplayLinkCallback, self);
		CVDisplayLinkStart(display_link);
	}
}

-(void)stopAnimating {
	if(display_link) {
		CVDisplayLinkRelease(display_link);
		display_link = nil;
	}
}

-(void)keyDown:(NSEvent*)event {
	if(event.characters.length)
		self.appWindow->keyDown([event.characters characterAtIndex:0], event.keyCode, (int)event.modifierFlags);
}

-(void)keyUp:(NSEvent*)event {
	if(event.characters.length)
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
