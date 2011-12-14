#import "AppDelegate.h"
#import "poOpenGLView.h"

#include <map>
#include "poWindow.h"
#include "poHelpers.h"

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
	window_settings = [[NSMutableDictionary alloc] init];
	
	// initialize the time
	poGetElapsedTime();
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

-(void)windowWillClose:(NSNotification*)notice {
	NSWindow *window = notice.object;
    
	NSView *view = window.contentView;
	window.contentView = nil;
    
	[view release];
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
	GLint swapInt = 1;
	[context setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];	
	// make our window, using the opengl context we just made
	poWindow *powin = new poWindow(str, appId, rectFromNSRect(frame));
	// and let it go
	CGLUnlockContext(cglcontext);
	
	// create the window
	NSWindow *window = [[KeyWindow alloc] initWithContentRect:frame
													styleMask:style_mask
													  backing:NSBackingStoreBuffered
														defer:YES];
    
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];
	
	[window setFrameOrigin:frame.origin];
	
	// configure the window a little
	[window setAcceptsMouseMovedEvents:YES];
	[window setReleasedWhenClosed:YES];
    
	// in case its full screen
	if(type == WINDOW_TYPE_FULLSCREEN) {
		[window setLevel:NSMainMenuWindowLevel+1];
		[window setOpaque:YES];
		[window setHidesOnDeactivate:YES];
		
		powin->setFullscreen(true);
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
	[opengl release];
    
	[window makeKeyAndOrderFront:self];
	
	return powin;
}

-(void)closeWindow:(poWindow*)window {
	[(NSWindow*)window->getWindowHandle() close];
}

-(void)fullscreenWindow:(NSWindow*)window {
	NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
						  [NSValue valueWithRect:[window frame]], @"WINDOW_FRAME",
						  nil];
	[window_settings setObject:dict forKey:[NSValue valueWithNonretainedObject:window]];
	
	NSScreen *screen = [window deepestScreen];
	
	[window setStyleMask:NSBorderlessWindowMask];
	[window setFrame:[screen frame] display:YES animate:NO];
    
	[window setLevel:NSMainMenuWindowLevel+1];
	[window setOpaque:YES];
	[window setHidesOnDeactivate:YES];
}

-(void)restoreWindow:(NSWindow*)window {
	NSDictionary *dict = [window_settings objectForKey:[NSValue valueWithNonretainedObject:window]];
	NSRect frame = [[dict objectForKey:@"WINDOW_FRAME"] rectValue];
	[window_settings removeObjectForKey:[NSValue valueWithNonretainedObject:window]];
	
	[window setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
	[window setFrame:frame display:YES animate:NO];
	[window setFrameOrigin:frame.origin];
	
	[window setLevel:NSNormalWindowLevel];
	[window setOpaque:NO];
	[window setHidesOnDeactivate:NO];
}

-(void)fullscreenWindow:(poWindow*)window value:(BOOL)b {
	window->setFullscreen(b);
    
	NSWindow *win = (NSWindow*)window->getWindowHandle();
	
	SEL function = @selector(fullscreenWindow:);
	if(!b)
		function = @selector(restoreWindow:);
	
	[self performSelectorOnMainThread:function withObject:win waitUntilDone:NO];
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

std::string applicationGetSupportDirectory() {
	NSString *dir = [NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES) lastObject];
	if (![[NSFileManager defaultManager] fileExistsAtPath:dir])
		[[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil];
	return [dir UTF8String];
}

void applicationMakeWindowCurrent(poWindow* win) {
	AppDelegate *app = [NSApplication sharedApplication].delegate;
	app.currentWindow = win;
}

void applicationMakeWindowFullscreen(poWindow* win, bool value) {
	if(win->isFullscreen() != value) {
		AppDelegate *app = [NSApplication sharedApplication].delegate;
		[app fullscreenWindow:win value:value];
	}
}

void applicationMoveWindow(poWindow* win, poPoint p) {
	NSWindow *window = (NSWindow*)win->getWindowHandle();
	[window setFrameOrigin:NSMakePoint(p.x, p.y)];
}

void applicationReshapeWindow(poWindow* win, poRect r) {
	NSWindow *window = (NSWindow*)win->getWindowHandle();
	NSRect new_bounds = NSMakeRect(window.frame.origin.x, window.frame.origin.y, r.width, r.height);
	NSRect new_frame = [NSWindow frameRectForContentRect:new_bounds styleMask:window.styleMask];
	[window setFrame:new_frame display:YES];
}
