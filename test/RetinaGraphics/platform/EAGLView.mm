//
//  EAGLView.m
//  OpenGLES_iPhone
//
//  Created by mmalc Crawford on 11/18/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import "EAGLView.h"
#import <QuartzCore/QuartzCore.h>
#include "poWindow.h"

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
@end

@implementation EAGLView

@synthesize context;
@synthesize size;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id) initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
	if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.backgroundColor =  CGColorCreate(CGColorSpaceCreateDeviceRGB(), (CGFloat[]){ 0,0,0, 1 });
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        //Enable multitouch
        self.multipleTouchEnabled = YES;
    }
    return self;
}

- (void)dealloc
{
    [self deleteFramebuffer];
    [context release];
    
    [super dealloc];
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext) {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer) {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

        glGenFramebuffers(1, multisampleBuffers);
        glBindFramebuffer(GL_FRAMEBUFFER, multisampleBuffers[0]);
        glGenRenderbuffers(2, multisampleBuffers+1);
        glBindRenderbuffer(GL_RENDERBUFFER, multisampleBuffers[1]);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 2, GL_RGBA8_OES, framebufferWidth, framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multisampleBuffers[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, multisampleBuffers[2]);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 2, GL_DEPTH24_STENCIL8_OES, framebufferWidth, framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multisampleBuffers[2]);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multisampleBuffers[2]);
        
        NSLog(@"Multisampling!");
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		
		[[NSNotificationCenter defaultCenter] postNotificationName:EAGLViewLayoutChangedNotification object:self];
    }
}

- (void)deleteFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
		
        glDeleteFramebuffers(1, multisampleBuffers);
        glDeleteRenderbuffers(2, multisampleBuffers+1);
    }
}

- (void)setFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, multisampleBuffers[0]);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context) {
        [EAGLContext setCurrentContext:context];
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, multisampleBuffers[0]);
        glResolveMultisampleFramebufferAPPLE();
        
        const GLenum discards[]  = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT};
        glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE,3,discards);
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);

		success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews {
    // Set the scale factor to be the same as the main screen
    if ([self respondsToSelector: NSSelectorFromString(@"contentScaleFactor")]) {
        [self setContentScaleFactor:[[UIScreen mainScreen] scale]];
    }
    
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

- (CGSize)size {
	return CGSizeMake(framebufferWidth, framebufferHeight);
}

@end
