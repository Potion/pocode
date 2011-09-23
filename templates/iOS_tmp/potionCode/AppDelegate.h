//
//  potionCodeAppDelegate.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <UIKit/UIKit.h>

class poWindow;

@class potionCodeViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet potionCodeViewController *viewController;
@property (nonatomic, readonly) poWindow *appWindow;

@end
