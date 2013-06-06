//
//  SMCAlertWindow.m
//  SMCApp
//
//  Created by Stephen Varga on 11/15/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#import "poUIAlertWindow.h"
#include "poDictionary.h"

@implementation poUIAlertWindow

- (void) show:(NSString*)title withMessage:(NSString*)message usingHandler:(po::Object*)handlerObject withHandlerMessage:(std::string)handlerMsg {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
    [alert show];
    [alert release];
    
    handler = handlerObject;
    self->handlerMessage = handlerMsg;
}


- (void)alertView:(UIAlertView *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    po::Dictionary dict;
    dict.set("index", buttonIndex);
    handler->messageHandler(handlerMessage, dict);
}

@end
