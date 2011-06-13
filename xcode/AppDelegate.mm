#import "AppDelegate.h"
#import "poOpenGLView.h"

#include "poApplication.h"
#include "poWindow.h"
#include "Helpers.h"

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
	
	NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
												   styleMask:style_mask
													 backing:NSBackingStoreBuffered
													   defer:NO
													  screen:screen];
	
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
	
	[context makeCurrentContext];
	poWindow *powin = new poWindow(str, 
								   window, 
								   appId, 
								   poRect(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height));
	
	poOpenGLView *opengl = [[poOpenGLView alloc] initWithFrame:frame
													   context:context
														window:powin];
	
	window.contentView = [opengl autorelease];
	[window makeKeyAndOrderFront:nil];
	[window setAcceptsMouseMovedEvents:YES];
	[window setReleasedWhenClosed:YES];
	
	if(type == WINDOW_TYPE_FULLSCREEN)
		powin->fullscreen(true);
	
	[windows addObject:opengl];
	return window;
}

-(int)numberWindows {
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

-(void)closeWindow:(NSWindow*)win {
	NSWindow *window = (NSWindow*)win;
	[windows removeObject:window];
	[window close];
}

-(void)fullscreenWindow:(NSWindow*)win value:(BOOL)b {
	NSWindow *window = (NSWindow*)win;
	[window.contentView setFullscreen:b];
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
	[window setFrame:NSMakeRect(r.origin.x, r.origin.y, r.size.x, r.size.y) display:YES];
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

