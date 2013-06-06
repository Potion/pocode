#pragma once
//
//  SMCAlertWindow.h
//  SMCApp
//
//  Created by Stephen Varga on 11/15/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//
#include "poObject.h"
#import <Foundation/Foundation.h>

@interface poUIAlertWindow : NSObject <UIAlertViewDelegate> {
    @public
    int             pressedButton;
    po::Object*     handler;
    std::string     handlerMessage;
}

- (void) show:(NSString*)title withMessage:(NSString*)message usingHandler:(po::Object*)handlerObject withHandlerMessage:(std::string)handlerMsg;

@end

