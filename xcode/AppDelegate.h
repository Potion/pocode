#import <Cocoa/Cocoa.h>

#include "poApplication.h"

class poWindow;

@interface AppDelegate : NSObject <NSApplicationDelegate> 
{
	NSMutableArray *windows;
	
	NSOpenGLPixelFormat *pixel_format;
	NSOpenGLContext *shared_context;
}

@property (nonatomic,assign) poWindow *currentWindow;

-(void)quit;
-(NSWindow*)createWindow:(uint)root_obj_id 
				type:(poWindowType)type 
			   frame:(NSRect)frame
			   title:(const char*)str;
-(int)numberWindows;
-(NSWindow*)getWindowByIndex:(int)index;
-(NSWindow*)getWindowByAppWin:(poWindow*)win;
-(void)closeWindow:(NSWindow*)win;
-(void)fullscreenWindow:(NSWindow*)win value:(BOOL)b;

@end
