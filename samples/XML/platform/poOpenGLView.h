//
//  poOpenGLView.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>

class poWindow;

@interface poOpenGLView : NSOpenGLView {
	CVDisplayLinkRef display_link;
	BOOL fullscreen;
}

@property (nonatomic,assign) poWindow *appWindow;
@property (nonatomic,assign) BOOL fullscreen;

-(id)initWithFrame:(NSRect)frm context:(NSOpenGLContext*)cxt window:(poWindow*)win;

-(BOOL)isAnimating;
-(void)startAnimating;
-(void)stopAnimating;

@end
