//
//  potionCodeAppDelegate.m
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import "AppDelegate.h"
#import "EAGLView.h"
#import "potionCodeViewController.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// initialize the time
	poGetCurrentTime();
	
	// move the pwd to match our present location
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:[[NSBundle mainBundle] resourcePath]];

	self.viewController.appWindow->setWindowHandle(self.window);
	self.window.rootViewController = self.viewController;
	
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	[self.viewController stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	/*
	 Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	 If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	 */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	/*
	 Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	 */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	/*
	 Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
	 */
	[self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[self.viewController stopAnimation];
}

- (void)dealloc
{
	[_window release];
	[_viewController release];
    [super dealloc];
}

@end




void applicationQuit() {
	NSLog(@"can't exit an iPhone app");
}

poWindow* applicationCreateWindow(uint root_id, poWindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.viewController.appWindow;
}

int applicationNumberWindows() {
	return 1;
}

poWindow* applicationGetWindow(int index) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.viewController.appWindow;
}

poWindow* applicationCurrentWindow() {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.viewController.appWindow;
}

void applicationMakeWindowCurrent(poWindow* win) {
}

void applicationMakeWindowFullscreen(poWindow* win, bool value) {
}

void applicationMoveWindow(poWindow* win, poPoint p) {
}

void applicationReshapeWindow(poWindow* win, poRect r) {
}

std::string applicationGetResourceDirectory() {
    NSString *path = [[NSBundle mainBundle] resourcePath];
    return [path UTF8String];
}

std::string applicationGetSupportDirectory() {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    return [documentsDirectoryPath UTF8String];
}



