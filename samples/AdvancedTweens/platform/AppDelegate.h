#import <Cocoa/Cocoa.h>

#include "poApplication.h"

class poWindow;

@interface AppDelegate : NSObject <NSApplicationDelegate> 
{
	NSOpenGLContext *shared_context;
	NSMutableDictionary *window_settings;

	poWindow *currentWindow;
}

@property (nonatomic,assign) poWindow *currentWindow;

-(void)quit;
-(poWindow*)createWindow:(uint)root_obj_id 
					type:(poWindowType)type 
				   frame:(NSRect)frame
				   title:(const char*)str;

-(void)closeWindow:(poWindow*)win;
-(void)fullscreenWindow:(poWindow*)win value:(BOOL)b;

@end
