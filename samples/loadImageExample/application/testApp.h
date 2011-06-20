#pragma once

#include "poObject.h"
#include "poImage.h"
#include "poShapeBasics2D.h"

class testApp : public poObject {
public:
	testApp();
	virtual ~testApp();
    
    virtual void draw();
    virtual void update();
    
    poImage* myImage;
    poRectShape* image;
    
    
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

};