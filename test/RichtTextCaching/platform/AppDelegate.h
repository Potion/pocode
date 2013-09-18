#import <Cocoa/Cocoa.h>

#include "poApplication.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
	NSOpenGLContext *shared_context;
	NSMutableDictionary *window_settings;
    
    po::Window *currentWindow;
}

@property (nonatomic,assign) po::Window *currentWindow;

-(void)quit;
-(po::Window*)createWindow:(uint)root_obj_id
type:(po::WindowType)type
frame:(NSRect)frame
title:(const char*)str;

-(void)closeWindow:(po::Window*)win;
-(void)fullscreenWindow:(po::Window*)win value:(BOOL)b;

@end
