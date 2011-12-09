//
//  poVideo.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/29/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "common_include.h"

class poVideoCapture {
public:
	poVideoCapture();
	~poVideoCapture();
	
private:
	void *impl;
};

class poVideo {
	
private:
	void *impl;
};