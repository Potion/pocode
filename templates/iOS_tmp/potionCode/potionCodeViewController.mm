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
#include "Helpers.h"
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
<<<<<<< HEAD
=======
	[touchTracker release];
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
	
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidLoad {
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
	
	touchTracker = [[NSMutableArray alloc] init];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
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

- (void)startAnimation
{
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
    [eagl setFramebuffer];
	
    self.appWindow->update();
	self.appWindow->draw();
	
    [eagl presentFramebuffer];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return YES;
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
		int touchId = 0;
		if([touchTracker containsObject:touch]) {
			touchId = [touchTracker indexOfObject:touch];
		}
		else {
			touchId = touchTracker.count;
			[touchTracker addObject:touch];
		}
		
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchBegin(touchPoint.x, touchPoint.y, touchId, touch.tapCount);
    }
} 

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
<<<<<<< HEAD
=======
        int touchId = [touchTracker indexOfObject:touch];
		
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchMove(touchPoint.x, touchPoint.y, touchId, touch.tapCount);
    }
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
		int touchId = [touchTracker indexOfObject:touch];
		[touchTracker removeObject:touch];
		
        CGPoint touchPoint = [touch locationInView:eagl];
        self.appWindow->touchEnd(touchPoint.x, touchPoint.y, touchId, touch.tapCount);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    for(UITouch *touch in touches) {
		int touchId = [touchTracker indexOfObject:touch];
		[touchTracker removeObject:touch];

        CGPoint touchPoint = [touch locationInView:eagl];
<<<<<<< HEAD
        self.appWindow->touchCancelled(touchPoint.x, touchPoint.y, (int)touch, touch.tapCount);
=======
        self.appWindow->touchCancelled(touchPoint.x, touchPoint.y, touchId, touch.tapCount);
>>>>>>> a88b709ee3d8a583ccdbe607650fb6d0d2f1955f
    }
	
}

@end
