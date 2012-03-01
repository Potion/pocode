//
//  EAGLView.h
//  OpenGLES_iPhone
//
//  Created by mmalc Crawford on 11/18/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <MessageUI/MessageUI.h>
#import <MediaPlayer/MediaPlayer.h>


static NSString * const EAGLViewLayoutChangedNotification = @"EAGL_VIEW_LAYOUT_CHANGED_NOTIFICTATION";

@class ViewControllerPhone;
@class ViewControllerPad;
@class EAGLContext;

@interface EAGLView : UIView <MPMediaPickerControllerDelegate> {
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint defaultFramebuffer, colorRenderbuffer, depthStencil;
	GLuint multisampleBuffers[3];
}

@property (nonatomic, retain) IBOutlet UIViewController *viewController;

@property (nonatomic,retain) EAGLContext *context;
@property (nonatomic,readonly) CGSize size;

- (void)setFramebuffer;
- (BOOL)presentFramebuffer;
- (void)deleteFramebuffer;



@end

