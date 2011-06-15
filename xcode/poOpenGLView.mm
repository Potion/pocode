//
//  poOpenGLView.m
//  poFramework4
//
//  Created by Joshua Fisher on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import "poOpenGLView.h"
#include "poWindow.h"

CVReturn MyDisplayLinkCallback (CVDisplayLinkRef displayLink,
								const CVTimeStamp *inNow,
								const CVTimeStamp *inOutputTime,
								CVOptionFlags flagsIn,
								CVOptionFlags *flagsOut,
								void *displayLinkContext)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	poOpenGLView *self = (poOpenGLView*)displayLinkContext;
	[self.openGLContext makeCurrentContext];
	if(self.appWindow && CVDisplayLinkIsRunning(displayLink)) {
		self.appWindow->makeCurrent();
		self.appWindow->update();
		self.appWindow->draw();
	}
	[self.openGLContext flushBuffer];
	
	[pool release];
	return kCVReturnSuccess;
}

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
		NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
		0
	};
	return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(id)initWithFrame:(NSRect)frm 
		   context:(NSOpenGLContext*)cxt
			window:(poWindow*)win
{
	self = [super initWithFrame:frm pixelFormat:[poOpenGLView defaultPixelFormat]];
	if(self) {
		self.openGLContext = cxt;
		
		[self.openGLContext makeCurrentContext];
		self.appWindow = win;
		
		display_link = nil;
		animating = YES;
	}
	return self;
}

-(void)dealloc {
	delete self.appWindow;
	self.appWindow = nil;
	[super dealloc];
}

-(BOOL)acceptsFirstResponder {
	return YES;
}

-(BOOL)isFlipped {
	return YES;
}

-(void)viewDidMoveToWindow {
	// we were either added or removed from a window
	if(self.window && animating) {
		[self startAnimating];
	}
	else {
		BOOL tmp = animating;
		[self stopAnimating];
		animating = tmp;
	}
}

-(BOOL)isFullscreen {
	return fullscreen;
}

-(void)setFullscreen:(BOOL)value {
	if(fullscreen != value) {
		fullscreen = value;

		if(fullscreen) {
			NSScreen *screen = self.window.deepestScreen;
			[self enterFullScreenMode:screen withOptions:[NSDictionary dictionary]];
		}
		else {
			[self exitFullScreenModeWithOptions:[NSDictionary dictionary]];
		}
	}
}

-(BOOL)isAnimating {
	return animating;
}

-(void)startAnimating {
	if(!display_link) {
		CGDirectDisplayID displayID = (CGDirectDisplayID)[[[[self.window screen] deviceDescription] objectForKey:@"NSScreenNumber"] intValue];
		CVDisplayLinkCreateWithCGDisplay(displayID, &display_link);
		CVDisplayLinkSetOutputCallback(display_link, MyDisplayLinkCallback, self);
		CVDisplayLinkStart(display_link);
		animating = YES;
	}
}

-(void)stopAnimating {
	if(display_link) {
		CVDisplayLinkStop(display_link);
		CVDisplayLinkRelease(display_link);
		display_link = nil;
		animating = NO;
	}
}

-(void)viewWillStartLiveResize {
	[self stopAnimating];
}

-(void)viewDidEndLiveResize {
	NSRect rect = self.bounds;
	self.appWindow->resize(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	[self startAnimating];
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
