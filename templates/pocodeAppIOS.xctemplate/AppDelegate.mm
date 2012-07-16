//
//  potionCodeAppDelegate.m
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//
#include <iostream.h>
#import "AppDelegate.h"
#import "EAGLView.h"
#import "potionCodeViewController.h"
#include "poApplication.h"

@interface MyVC : UIViewController
@end



@implementation MyVC : UIViewController

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    
    poOrientation poCodeOrientation = PO_UNKNOWN_ORIENTATION;
    switch (toInterfaceOrientation) {
        case UIInterfaceOrientationPortrait:
            poCodeOrientation = PO_VERTICAL_UP;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            poCodeOrientation = PO_VERTICAL_DOWN;
            break;
        case UIInterfaceOrientationLandscapeRight:
            poCodeOrientation = PO_HORIZONTAL_RIGHT;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            poCodeOrientation = PO_HORIZONTAL_LEFT;
            break;
    }
    
    //See if orientation is supported, if it is set it and return YES
    for(int i=0; i<app->poSupportedOrientations.size(); i++) {
        if(app->poSupportedOrientations[i] == poCodeOrientation) {
            app->poAppOrientation = poCodeOrientation;
            [app.pocodeVC rotationEvent];
            
            return YES;
        }
    }
    
    return NO;
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UIView *view in self.view.subviews) {
        if(view == [[self.view subviews] objectAtIndex:0]) {
            [view touchesBegan:touches withEvent:event];
            break;
        }
    }
    
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UIView *view in self.view.subviews) {
        if(view == [[self.view subviews] objectAtIndex:0]) {
            [view touchesMoved:touches withEvent:event];
            break;
        }
    }
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UIView *view in self.view.subviews) {
        if(view == [[self.view subviews] objectAtIndex:0]) {
            [view touchesEnded:touches withEvent:event];
            break;
        }
    }
}

@end

@implementation AppDelegate

@synthesize window, pocodeVC;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    //Run init methods
    poAppOrientation = PO_VERTICAL_UP;
    setupApplication();
    
	// initialize the time
	poGetElapsedTime();
	
	// move the pwd to match our present location
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:[[NSBundle mainBundle] resourcePath]];
    
	UIView *rootView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	
    NSBundle* mainBundle = [NSBundle mainBundle];
    
    //Set Splash Screen
    NSString *defaultImage;
    
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        if(UIInterfaceOrientationIsLandscape([UIDevice currentDevice].orientation)) {
            defaultImage = @"Default-Landscape.png";
        } else {
            defaultImage = @"Default-Portrait.png";
        }
    } else if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        bool bVerticalSupported = false;
        for(int i=0; i<poSupportedOrientations.size(); i++) {
            if(poSupportedOrientations[i] == PO_VERTICAL_UP || poSupportedOrientations[i] == PO_VERTICAL_DOWN) {
                bVerticalSupported = true; 
                break;
            }
        }
        if(!bVerticalSupported) {
            defaultImage = @"DefaultiphoneLanscapeRight.png";
            NSLog(@"Right Orientation");
        } else {
            defaultImage = @"Default~iphone.png";
        }
    }
    
    UIDeviceOrientation   orientation = [UIDevice currentDevice].orientation;
    NSString   *nibName = UIDeviceOrientationIsLandscape(orientation) ? @"Landscape" : @"Portrait";
    NSLog(@"orientation is %@",nibName);
    
	UIImage *img = [UIImage imageNamed:defaultImage];
	fakeSplash = [[UIImageView alloc] initWithImage:img];
	[rootView addSubview:fakeSplash];
	[fakeSplash release]; 
	
	UIViewController *root = [[MyVC alloc] init];
	root.view = rootView;
	[rootView release];
    
    self.window.backgroundColor     = [UIColor blackColor];
	self.window.rootViewController  = root;
	[root release];
    
    [self performSelector:@selector(loadGLVC) withObject:nil afterDelay:0.1];
	
    return YES;
}



-(void)loadGLVC {
    self.pocodeVC = [[potionCodeViewController alloc] init];
	self.pocodeVC.appWindow->setWindowHandle(self.window);
    self.pocodeVC.eagl.multipleTouchEnabled = YES;
	[self.window.rootViewController.view insertSubview:self.pocodeVC.view belowSubview:fakeSplash];
    
	[self.pocodeVC startAnimation];
	[self performSelector:@selector(removeFakeSplash) withObject:nil afterDelay:0.1];
}

-(void)removeFakeSplash {
	[UIView animateWithDuration:0.5 animations:^{
		fakeSplash.alpha = 0.0;
	} completion:^(BOOL finished) {
		[fakeSplash removeFromSuperview];
		fakeSplash = nil;
	}];
}

- (void)applicationWillResignActive:(UIApplication *)application {
	[self.pocodeVC stopAnimation];
    
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    __block UIBackgroundTaskIdentifier background_task; //Create a task object
    
    background_task = [application beginBackgroundTaskWithExpirationHandler: ^ {
        [application endBackgroundTask: background_task]; //Tell the system that we are done with the tasks
        background_task = UIBackgroundTaskInvalid; //Set the task to be invalid
        
        //System will be shutting down the app at any point in time now
    }];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	/*
	 Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	 */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    [self.pocodeVC startAnimation];
	
}

- (void)applicationWillTerminate:(UIApplication *)application {
	[self.pocodeVC stopAnimation];
}

- (void)dealloc {
	[window release];
    
    [pocodeVC.view removeFromSuperview];
	[pocodeVC release];
    
    [super dealloc];
}

@end




void applicationQuit() {
	NSLog(@"can't exit an iPhone app");
}

poWindow* applicationCreateWindow(uint root_id, poWindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

int applicationNumberWindows() {
	return 1;
}

poWindow* applicationGetWindow(int index) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

poWindow* applicationCurrentWindow() {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

std::string applicationGetResourceDirectory() {
	return [[[NSBundle mainBundle] resourcePath] UTF8String];
}

std::string applicationGetSupportDirectory() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSArray *dirPaths;
    NSString *docsDir;
    
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    docsDir = [dirPaths objectAtIndex:0];
    
    std::string dirStr = [docsDir UTF8String];
    
    [pool drain];
    
    return  dirStr + "/"; 
}

//Not implemented on iOS
void applicationMakeWindowCurrent(poWindow* win) {}
void applicationMakeWindowFullscreen(poWindow* win, bool value) {}
void applicationMoveWindow(poWindow* win, poPoint p) {}
void applicationReshapeWindow(poWindow* win, poRect r) {}

//MultiTouch
void poSetMultiTouchEnabled(bool isEnabled) {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    app.pocodeVC.eagl.multipleTouchEnabled = isEnabled;
}

//Accelerometer 
void poStartAccelerometer(float frequency) {
    UIAccelerometer*  theAccelerometer = [UIAccelerometer sharedAccelerometer];
    theAccelerometer.updateInterval = 1 / frequency;
    
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    theAccelerometer.delegate = app.pocodeVC;
}

void poStopAccelerometer() {
    UIAccelerometer* theAccelerometer = [UIAccelerometer sharedAccelerometer];
    theAccelerometer.delegate = nil;
}


//Orientation
poOrientation poGetOrientation() {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app->poAppOrientation;
}

void poSetAutoRotateOrientations(unsigned char orientations) {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    app->poSupportedOrientations.clear();
    
    if(orientations & PO_VERTICAL_UP)      app->poSupportedOrientations.push_back(PO_VERTICAL_UP);
    if(orientations & PO_VERTICAL_DOWN)    app->poSupportedOrientations.push_back(PO_VERTICAL_DOWN);
    
    if(orientations & PO_HORIZONTAL_RIGHT) app->poSupportedOrientations.push_back(PO_HORIZONTAL_RIGHT);
    if(orientations & PO_HORIZONTAL_LEFT)  app->poSupportedOrientations.push_back(PO_HORIZONTAL_LEFT);
}

