#pragma once

#include "poShape2D.h"

class testApp : public poObject {
public:
	testApp();
	virtual ~testApp();
	
    poImage* myImage;
    poShape2D* image;
};