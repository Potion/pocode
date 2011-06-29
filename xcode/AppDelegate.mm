#import "AppDelegate.h"
#import "poOpenGLView.h"

#include <map>
#include "poWindow.h"
#include "Helpers.h"

// have to override this as a borderless window returns NO for canBecomeKeyWindow
@interface KeyFullscreenWindow : NSWindow {}
@end
@implementation KeyFullscreenWindow
-(BOOL)canBecomeKeyWindow {return YES;}
@end

std::map<NSView*,NSDictionary*> windows_fullscreen_restore;

@implementation AppDelegate

@synthesize currentWindow;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	getTime();
	
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:[[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]];
	
	// setup variables
	self.currentWindow = nil;
	shared_context = nil;
	windows = [[NSMutableArray alloc] init];
	
	// and  setup the application
	setupApplication();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	cleanupApplication();
	[windows release];
	windows = nil;
	[shared_context release];
}

- (void)windowWillClose:(NSNotification*)notice {
	if(windows) {
		NSWindow *win = (NSWindow*)notice;
		[windows removeObject:win];
	}
}

-(void)quit {
	[[NSApplication sharedApplication] terminate:nil];
}

-(NSWindow*)createWindow:(uint)appId 
					type:(poWindowType)type 
				   frame:(NSRect)frame 
				   title:(const char*)str
{
	NSScreen *screen = nil;
	NSEnumerator *screens = [[NSScreen screens] objectEnumerator];
	while((screen = [screens nextObject]) && !NSPointInRect(frame.origin, [screen frame])) {}
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
	
	NSOpenGLContext *context = nil;
	
	if(!shared_context) {
		shared_context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
													shareContext:nil];
		context = shared_context;
	}
	else {
		context = [[NSOpenGLContext alloc] initWithFormat:[poOpenGLView defaultPixelFormat]
											 shareContext:shared_context];
	}
	GLint swapInt = 1;
    [context setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];	
	[context makeCurrentContext];
	
	NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
												   styleMask:style_mask
													 backing:NSBackingStoreBuffered
													   defer:YES
													  screen:screen];
	[window setFrameOrigin:frame.origin];
	
	poWindow *powin = new poWindow(str, 
								   window, 
								   appId, 
								   poRect(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height));
	
	poOpenGLView *opengl = [[poOpenGLView alloc] initWithFrame:frame
													   context:context
														window:powin];
	
	NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
	[center addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];
	
	window.contentView = [opengl autorelease];
	[window makeKeyAndOrderFront:nil];
	[window setAcceptsMouseMovedEvents:YES];
	[window setReleasedWhenClosed:YES];
	[windows addObject:window];

	if(type == WINDOW_TYPE_FULLSCREEN)
		[self fullscreenWindow:window value:YES];
	
	return window;
}

-(NSUInteger)numberWindows {
	return windows.count;
}

-(NSWindow*)getWindowByIndex:(int)index {
	return [windows objectAtIndex:index];
}

-(NSWindow*)getWindowByAppWin:(poWindow*)win {
	for(NSWindow *window in windows) {
		poOpenGLView *glview = window.contentView;
		if(glview.appWindow == win)
			return window;
	}
	return nil;
}

-(void)closeWindow:(NSWindow*)window {
	[window close];
}

-(void)fullscreenWindow:(NSWindow*)win value:(BOOL)b {
	if(b == YES) {
		NSScreen *screen = win.screen;
		NSWindow *window = [[KeyFullscreenWindow alloc] initWithContentRect:screen.frame
													   styleMask:NSBorderlessWindowMask
														 backing:NSBackingStoreBuffered
														   defer:YES];
		[window setLevel:CGShieldingWindowLevel()+1];
		[window setOpaque:YES];
		[window setHidesOnDeactivate:YES];
		
		window.contentView = win.contentView;
		window.initialFirstResponder = win.contentView;
		
		[window setAcceptsMouseMovedEvents:YES];
		[window makeKeyAndOrderFront:self];
		[window setReleasedWhenClosed:YES];

		// keep this window around
		[windows addObject:window];
		
		// save the old window size
		NSDictionary *dict = [[NSDictionary alloc] initWithObjectsAndKeys:
							  [NSValue valueWithRect:win.frame], @"frame",
							  [NSNumber numberWithUnsignedInteger:win.styleMask], @"style",
							  nil];
		windows_fullscreen_restore[window.contentView] = dict;
		[win close];
		
		CGDirectDisplayID display_id = [[screen.deviceDescription objectForKey:@"NSScreenNumber"] unsignedIntValue];
		CGDisplayCapture(display_id);
	}
	else {
		// release the screen
		CGDirectDisplayID display_id = [[win.screen.deviceDescription objectForKey:@"NSScreenNumber"] unsignedIntValue];
		CGDisplayRelease(display_id);

		// get the stored window properties
		NSDictionary *dict = windows_fullscreen_restore[win.contentView];
		
		NSRect rect = [[dict valueForKey:@"frame"] rectValue];
		NSUInteger style = [[dict valueForKey:@"style"] unsignedIntegerValue];
		
		// clean up the dictionary
		[dict release];
		// and destroy the record
		windows_fullscreen_restore[win.contentView] = nil;

		NSWindow *window = [[NSWindow alloc] initWithContentRect:rect
													   styleMask:style
														 backing:NSBackingStoreBuffered
														   defer:YES];
		
		NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
		[center addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];

		window.contentView = win.contentView;
		window.initialFirstResponder = window.contentView;

		[window makeKeyAndOrderFront:self];
		[window setAcceptsMouseMovedEvents:YES];
		[window setReleasedWhenClosed:YES];

		[windows addObject:window];
		[win close];
	}
}

@end


void applicationQuit() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	[app quit];
}

poWindow* applicationCreateWindow(uint root_id, poWindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	NSWindow *window = [app createWindow:root_id type:type frame:NSMakeRect(x,y,w,h) title:title];
	poOpenGLView *glview = window.contentView;
	return glview.appWindow;
}

int applicationNumberWindows() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return [app numberWindows];
}

poWindow* applicationGetWindow(int index) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	NSWindow *window = [app getWindowByIndex:index];
	poOpenGLView *glview = window.contentView;
	return glview.appWindow;
}

poWindow* applicationCurrentWindow() {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	return app.currentWindow;
}

void applicationMakeWindowCurrent(poWindow* win) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	app.currentWindow = win;
}

void applicationMoveWindow(poWindow* win, poRect r) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	NSWindow *window = [app getWindowByAppWin:win];
	[window setFrame:NSMakeRect(0, 0, r.size.x, r.size.y) display:YES];
	[window setFrameOrigin:CGPointMake(r.origin.x, r.origin.y)];
	
}

void applicationMakeWindowFullscreen(poWindow* win, bool value) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	NSWindow *window = [app getWindowByAppWin:win];
	[window.contentView setFullscreen:value];
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

