//
//  potionCodeViewController.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "poHelpers.h"
#include "poWindow.h"
#import "EAGLView.h"

@interface potionCodeViewController : UIViewController {
    EAGLContext *context;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
	
	NSMutableArray *touchTracker;
}

@property (nonatomic, assign) IBOutlet EAGLView *eagl;

@property (nonatomic, readonly) poWindow *appWindow;
@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void)startAnimation;
- (void)stopAnimation;

@end
