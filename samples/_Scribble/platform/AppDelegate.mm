#import "AppDelegate.h"
#import "poOpenGLView.h"

#include <map>
#include "poWindow.h"
#include "Helpers.h"

poRect rectFromNSRect(NSRect r) {
	return poRect(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

std::map<NSView*,NSDictionary*> windows_fullscreen_restore;

@interface KeyWindow : NSWindow {} @end
@implementation KeyWindow
-(BOOL)canBecomeKeyWindow {return YES;}
@end

@implementation AppDelegate
@synthesize currentWindow;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// initialize the time
	getTime();
	// move the pwd to match our present location
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:[[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]];
	// make sure we have a context to share
	shared_context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
												shareContext:nil];
	// and  setup the application
	self.currentWindow = nil;
	setupApplication();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	cleanupApplication();
	[shared_context release];
}

-(void)quit {
	[[NSApplication sharedApplication] terminate:nil];
}

-(poWindow*)createWindow:(uint)appId 
					type:(poWindowType)type 
				   frame:(NSRect)frame 
				   title:(const char*)str
{
	NSScreen *screen = nil;
	NSEnumerator *screens = [[NSScreen screens] objectEnumerator];
	while((screen = [screens nextObject])) {
		if(NSPointInRect(frame.origin, [screen frame]))
			break;
	}
	if(!screen) {
		screen = NSScreen.mainScreen;
	}
	
	NSUInteger style_mask;
	switch(type) {
		case WINDOW_TYPE_FULLSCREEN:
			frame = [screen frame];
			// we still want the borderless mask so let it fall thru
		case WINDOW_TYPE_BORDERLESS:
			style_mask = NSBorderlessWindowMask;
			break;
		case WINDOW_TYPE_NORMAL:
			style_mask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
			break;
	}
	
	// create the context
	NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
										 shareContext:shared_context];
	[context makeCurrentContext];
	// lock it so we can make our app window
	CGLContextObj cglcontext = (CGLContextObj)context.CGLContextObj;
	CGLLockContext(cglcontext);
	// make sure we're tied to the vsync
//	GLint swapInt = 1;
//	[context setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];	
	// make our window, using the opengl context we just made
	poWindow *powin = new poWindow(str, appId, rectFromNSRect(frame));
	// and let it go
	CGLUnlockContext(cglcontext);
	
	// create the window
	NSWindow *window = [[KeyWindow alloc] initWithContentRect:frame
													styleMask:style_mask
													  backing:NSBackingStoreBuffered
														defer:YES];
	[window setFrameOrigin:frame.origin];
	
	// configure the window a little
	[window setAcceptsMouseMovedEvents:YES];
	[window setReleasedWhenClosed:YES];

	// in case its full screen
	if(type == WINDOW_TYPE_FULLSCREEN) {
		[window setLevel:NSMainMenuWindowLevel+1];
		[window setOpaque:YES];
		[window setHidesOnDeactivate:YES];
	}

	NSRect glrect = frame;
	glrect.origin = NSMakePoint(0.f, 0.f);
	
	poOpenGLView *opengl = [[poOpenGLView alloc] initWithFrame:glrect];
	opengl.openGLContext = context;
	// give the gl view an app window to draw
	opengl.appWindow = powin;
	// and tell the app window which window its in
	powin->setWindowHandle(window);
	
	[window setContentView:opengl];
	[window makeKeyAndOrderFront:self];

	return powin;
}

-(void)closeWindow:(poWindow*)window {
	[(NSWindow*)window->getWindowHandle() close];
}

-(void)fullscreenWindow:(poWindow*)window value:(BOOL)b {
	NSWindow *win = (NSWindow*)window->getWindowHandle();

//	if(b == YES) {
//		// save the old window size
//		NSDictionary *dict = [[NSDictionary alloc] initWithObjectsAndKeys:
//							  [NSValue valueWithRect:win.frame], @"frame",
//							  [NSNumber numberWithUnsignedInteger:win.styleMask], @"style",
//							  nil];
//		windows_fullscreen_restore[win.contentView] = dict;
//		NSScreen *screen = win.screen;
//		
//		CGDirectDisplayID display_id = [[screen.deviceDescription objectForKey:@"NSScreenNumber"] unsignedIntValue];
//		CGDisplayCapture(display_id);
//
//		NSWindow *window = [[NSWindow alloc] initWithContentRect:screen.frame
//													   styleMask:NSBorderlessWindowMask
//														 backing:NSBackingStoreBuffered
//														   defer:YES];
//		[window setLevel:NSMainMenuWindowLevel+1];
//		[window setOpaque:YES];
//		[window setHidesOnDeactivate:YES];
//		
//		[window setAcceptsMouseMovedEvents:YES];
//		[window setReleasedWhenClosed:YES];
//		
//		[window makeKeyAndOrderFront:self];
//
//		// keep this window around
//		[windows addObject:window];
//		// and close the old one
//		[win close];
//	}
//	else {
//		// release the screen
//		CGDirectDisplayID display_id = [[win.screen.deviceDescription objectForKey:@"NSScreenNumber"] unsignedIntValue];
//		CGDisplayRelease(display_id);
//
//		// get the stored window properties
//		NSDictionary *dict = windows_fullscreen_restore[win.contentView];
//		
//		NSRect rect = [[dict valueForKey:@"frame"] rectValue];
//		NSUInteger style = [[dict valueForKey:@"style"] unsignedIntegerValue];
//		
//		// clean up the dictionary
//		[dict release];
//		// and destroy the record
//		windows_fullscreen_restore[win.contentView] = nil;
//
//		NSWindow *window = [[NSWindow alloc] initWithContentRect:rect
//													   styleMask:style
//														 backing:NSBackingStoreBuffered
//														   defer:YES];
//		
//		NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
//		[center addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];
//
//		window.contentView = win.contentView;
//		window.initialFirstResponder = window.contentView;
//
//		[window makeKeyAndOrderFront:self];
//		[window setAcceptsMouseMovedEvents:YES];
//		[window setReleasedWhenClosed:YES];
//
//		[windows addObject:window];
//		[win close];
//	}
}

@end


void applicationQuit() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	[app quit];
}

poWindow* applicationCreateWindow(uint root_id, poWindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return [app createWindow:root_id type:type frame:NSMakeRect(x,y,w,h) title:title];
}

int applicationNumberWindows() {
	return [NSApplication sharedApplication].windows.count;
}

poWindow* applicationGetWindow(int index) {
	NSWindow *window = [[NSApplication sharedApplication].windows objectAtIndex:index];
	return ((poOpenGLView*)window.contentView).appWindow;
}

poWindow* applicationCurrentWindow() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow;
}

void applicationMakeWindowCurrent(poWindow* win) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	app.currentWindow = win;
}

void applicationMakeWindowFullscreen(poWindow* win, bool value) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	[app fullscreenWindow:win value:value];
}

void applicationMoveWindow(poWindow* win, poPoint p) {
	NSWindow *window = (NSWindow*)win->getWindowHandle();
	[window setFrameOrigin:NSMakePoint(p.x, p.y)];
}

void applicationReshapeWindow(poWindow* win, poRect r) {
	NSWindow *window = (NSWindow*)win->getWindowHandle();
	NSRect new_bounds = NSMakeRect(window.frame.origin.x, window.frame.origin.y, r.width(), r.height());
	NSRect new_frame = [NSWindow frameRectForContentRect:new_bounds styleMask:window.styleMask];
	[window setFrame:new_frame display:YES];
}

float getWindowWidth() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->width();
}

float getWindowHeight() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->height();
}

poRect getWindowFrame() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->frame();
}

poRect getWindowBounds() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->bounds();
}

float getWindowFramerate() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->framerate();
}

float getWindowLastFrameTime() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->lastFrameTime();
}

float getWindowLastFrameDuration() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow->lastFrameElapsed();
}

