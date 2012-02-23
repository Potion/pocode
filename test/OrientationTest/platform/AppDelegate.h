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
        poOrientation               poAppOrientation;
        std::vector<poOrientation>  poSupportedOrientations;
    @protected
        bool isReachableAtEnterBackground;
        UIImageView *fakeSplash;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) potionCodeViewController *pocodeVC;

@end
