//
//  potionCodeViewController.m
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "potionCodeViewController.h"

#include "poObject.h"
#include "poHelpers.h"
#include "poApplication.h"

poObject *root = NULL;

@interface potionCodeViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation potionCodeViewController

@synthesize animating;
@synthesize context;
@synthesize displayLink;
@synthesize appWindow;
@synthesize eagl;

- (void)dealloc
{
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)loadView {
	eagl = [[EAGLView alloc] initWithFrame:CGRectMake(0,0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
	self.view = eagl;
	[eagl release];
    
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [eagl setContext:context];
    [eagl setFramebuffer];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(eaglViewLayoutChanged:) name:EAGLViewLayoutChangedNotification object:eagl];
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
}

- (void)viewWillAppear:(BOOL)animated {
    [self startAnimation];
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [self stopAnimation];
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload {
	[super viewDidUnload];
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval {
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval {
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation {
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation {
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame {
    [eagl setFramebuffer];
	
    self.appWindow->update();
	self.appWindow->draw();
	
    [eagl presentFramebuffer];
}

-(void)eaglViewLayoutChanged:(NSNotification*)notice {
	CGSize size = eagl.size;
	self.appWindow->resized(size.width, size.height);
}

- (poWindow*)appWindow {
	if(appWindow == NULL) {
		CGSize size = eagl.size;
		poRect frame(0, 0, size.width, size.height);
		appWindow = new poWindow("window", 0, frame);
	}
	return appWindow;
}

/*Touch events*/
- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchBegin(touchPoint.x, touchPoint.y, (int)touch, touch.tapCount);
    }
} 

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
        
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchMove(touchPoint.x, touchPoint.y, (int)touch, touch.tapCount);
    }
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchEnd(touchPoint.x, touchPoint.y, (int)touch, touch.tapCount);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchCancelled(touchPoint.x, touchPoint.y, (int)touch, touch.tapCount);
    }
}

//Motion Events
- (void) rotationEvent {	
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
    //Resize the GL View
    if(UIDeviceOrientationIsLandscape(orientation)) {
        [eagl setFrame:CGRectMake(0,0, [[UIScreen mainScreen] bounds].size.height, [[UIScreen mainScreen] bounds].size.width)];
    } else if(UIDeviceOrientationIsPortrait(orientation)) {
        [eagl setFrame:CGRectMake(0,0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    }
    
    self->appWindow->rotationEvent();
}


- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
    UIAccelerationValue x, y, z;
    switch (poGetOrientation()) {
        case PO_HORIZONTAL_LEFT:
            x = acceleration.y;
            y = acceleration.x;
            z = acceleration.z;
            break;
            
        case PO_HORIZONTAL_RIGHT:
            x = -acceleration.y;
            y = -acceleration.x;
            z = acceleration.z;
            break;
            
        case PO_VERTICAL_UP:
            x = acceleration.x;
            y = -acceleration.y;
            z = acceleration.z;
            break;
            
        case PO_VERTICAL_DOWN:
            x = -acceleration.x;
            y = acceleration.y;
            z = acceleration.z;
            break;
    }
    
    //Send Event
    self.appWindow->accelerometerEvent(x, y, z);
    
    // Do something with the values.
}

@end
