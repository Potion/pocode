#import "CocoaImpl.h"

@implementation CocoaAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	setupApplication();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	cleanupApplication();
}

@end
