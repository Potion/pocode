//
//  poVideo.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/29/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poVideo.h"
#include <QTKit/QTKit.h>

poVideoCapture::poVideoCapture() {
	NSError *error = nil;
	BOOL success = NO;

	QTCaptureSession* session = [[QTCaptureSession alloc] init];

	QTCaptureDevice *video_dev = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
	success = [video_dev open&error];
	
	QTCaptureDeviceInput *video_in = [[QTCaptureDeviceInput alloc] initWithDevice:video_dev];
	success = [session addInput:video_in error:&error];
}

poVideoCapture::~poVideoCapture() {
	[session release];
}
