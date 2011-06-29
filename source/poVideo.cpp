//
//  poVideo.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/29/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poVideo.h"
#include <QTKit/QTKit.h>

struct osxVideoCaptureImpl {
	QTCaptureSession *session;
	QTCaptureDeviceInput *device;
};

poVideoCapture::poVideoCapture() {
	osxVideoCaptureImpl *impl = new osxVideoCaptureImpl();
	
	NSError *error = nil;
	BOOL success = NO;

	impl->session = [[QTCaptureSession alloc] init];

	QTCaptureDevice *video_dev = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
	success = [video_dev open:&error];
	
	impl->device = [[QTCaptureDeviceInput alloc] initWithDevice:video_dev];
	success = [impl->session addInput:impl->device error:&error];
	
	this->impl = impl;
}

poVideoCapture::~poVideoCapture() {
	osxVideoCaptureImpl *impl = static_cast<osxVideoCaptureImpl*>(this->impl);
	[impl->session release];
}
