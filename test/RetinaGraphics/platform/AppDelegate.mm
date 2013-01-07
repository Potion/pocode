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
    
    po::Orientation poCodeOrientation = po::ORIENTATION_UNKNOWN;
    switch (toInterfaceOrientation) {
        case UIInterfaceOrientationPortrait:
            poCodeOrientation = po::ORIENTATION_VERTICAL_UP;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            poCodeOrientation = po::ORIENTATION_VERTICAL_DOWN;
            break;
        case UIInterfaceOrientationLandscapeRight:
            poCodeOrientation = po::ORIENTATION_HORIZONTAL_RIGHT;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            poCodeOrientation = po::ORIENTATION_HORIZONTAL_LEFT;
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
    poAppOrientation = po::ORIENTATION_VERTICAL_UP;
    po::setupApplication();
    
	// initialize the time
    po::getElapsedTime();
	
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
            if(poSupportedOrientations[i] == po::ORIENTATION_VERTICAL_UP || poSupportedOrientations[i] == po::ORIENTATION_VERTICAL_DOWN) {
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




void po::applicationQuit() {
	NSLog(@"can't exit an iPhone app");
}

po::Window* po::applicationCreateWindow(uint root_id, po::WindowType type, const char* title, int x, int y, int w, int h) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

int po::applicationNumberWindows() {
	return 1;
}

po::Window* po::applicationGetWindow(int index) {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

po::Window* po::applicationCurrentWindow() {
	AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app.pocodeVC.appWindow;
}

std::string po::applicationGetResourceDirectory() {
	return [[[NSBundle mainBundle] resourcePath] UTF8String];
}

std::string po::applicationGetSupportDirectory() {
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
void po::applicationMakeWindowCurrent(po::Window* win) {}
void po::applicationMakeWindowFullscreen(po::Window* win, bool value) {}
void po::applicationMoveWindow(po::Window* win, po::Point p) {}
void po::applicationReshapeWindow(po::Window* win, po::Rect r) {}

//MultiTouch
void po::setMultiTouchEnabled(bool isEnabled) {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    app.pocodeVC.eagl.multipleTouchEnabled = isEnabled;
}

//Accelerometer 
void po::startAccelerometer(float frequency) {
    UIAccelerometer*  theAccelerometer = [UIAccelerometer sharedAccelerometer];
    theAccelerometer.updateInterval = 1 / frequency;
    
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    theAccelerometer.delegate = app.pocodeVC;
}

void po::stopAccelerometer() {
    UIAccelerometer* theAccelerometer = [UIAccelerometer sharedAccelerometer];
    theAccelerometer.delegate = nil;
}


//Orientation
po::Orientation po::getOrientation() {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
	return app->poAppOrientation;
}

void po::setAutoRotateOrientations(unsigned char orientations) {
    AppDelegate *app = [UIApplication sharedApplication].delegate;
    app->poSupportedOrientations.clear();
    
    if(orientations & po::ORIENTATION_VERTICAL_UP)      app->poSupportedOrientations.push_back(po::ORIENTATION_VERTICAL_UP);
    if(orientations & po::ORIENTATION_VERTICAL_DOWN)    app->poSupportedOrientations.push_back(po::ORIENTATION_VERTICAL_DOWN);
    
    if(orientations & po::ORIENTATION_HORIZONTAL_RIGHT) app->poSupportedOrientations.push_back(po::ORIENTATION_HORIZONTAL_RIGHT);
    if(orientations & po::ORIENTATION_HORIZONTAL_LEFT)  app->poSupportedOrientations.push_back(po::ORIENTATION_HORIZONTAL_LEFT);
}

