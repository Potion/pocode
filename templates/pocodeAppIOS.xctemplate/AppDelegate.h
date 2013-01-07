//
//  potionCodeAppDelegate.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <vector>
#include "poEnums.h"

@class potionCodeViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
@public
    po::Orientation               poAppOrientation;
    std::vector<po::Orientation>  poSupportedOrientations;
@private
    double timeAtEnterBackground;
    bool isReachableAtEnterBackground;
    UIImageView *fakeSplash;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) potionCodeViewController *pocodeVC;

- (void)failIfSimulator;
- (void)handleNotification;
@end