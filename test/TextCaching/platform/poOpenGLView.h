//
//  poOpenGLView.h
//  pocode
//
//  Created by Joshua Fisher on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <Cocoa/Cocoa.h>

class poWindow;

@interface poOpenGLView : NSView {
@private
	CVDisplayLinkRef display_link;
	NSOpenGLContext *context;
	NSOpenGLPixelFormat *format;
	
	poWindow *appWindow;
}

@property (nonatomic,assign) poWindow *appWindow;
@property (nonatomic,retain) NSOpenGLContext *openGLContext;

+(NSOpenGLPixelFormat*)defaultPixelFormat;

-(id)initWithFrame:(NSRect)frm;

-(void)update;

-(BOOL)isAnimating;
-(void)startAnimating;
-(void)stopAnimating;

@end
