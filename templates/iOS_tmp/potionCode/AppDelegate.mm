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

#include "Helpers.h"
#include "poWindow.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;
@synthesize appWindow;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// initialize the time
	getTime();
	
	// move the pwd to match our present location
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:[[NSBundle mainBundle] resourcePath]];

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

- (poWindow*)appWindow {
	if(appWindow == NULL) {
		poRect frame(0, 0, 1024, 768);
		
		UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
		if(UIInterfaceOrientationIsPortrait(orientation))
			frame.setSize(768, 1024);
		
		appWindow = new poWindow("window window", 0, frame);
        appWindow->setWindowHandle(self.window);
	}
	return appWindow;
}

@end




void applicationQuit() {
	NSLog(@"can't exit an iPhone app");
}

poWindow* applicationCreateWindow(uint root_id, poWindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.appWindow;
}

int applicationNumberWindows() {
	return 1;
}

poWindow* applicationGetWindow(int index) {
}

poWindow* applicationCurrentWindow() {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.appWindow;
}

void applicationMakeWindowCurrent(poWindow* win) {
}

void applicationMakeWindowFullscreen(poWindow* win, bool value) {
}

void applicationMoveWindow(poWindow* win, poPoint p) {
}

void applicationReshapeWindow(poWindow* win, poRect r) {
}
