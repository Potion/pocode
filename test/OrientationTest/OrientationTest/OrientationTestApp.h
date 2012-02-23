/*	Created by Steve Varga on 2/21/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */
#include "poApplication.h"
#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"


class OrientationTestApp : public poObject {
public:
	OrientationTestApp();
	virtual ~OrientationTestApp();
	
    virtual void update();
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poRectShape* bg;
    
    poRectShape* arrow;
    
    //Circle with accelerometer
    poOvalShape* circle;
    poPoint accel;
    float friction;
    
    poTextBox* accelInfo;
    
    //Center the arrow to the screen orientation with transition
    void centerArrow();
    
    bool bLockOrientation;
};

