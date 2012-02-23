/*	Created by Steve Varga on 2/21/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include <iostream>
#include "OrientationTestApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poHelpers.h"

// APP CONSTRUCTOR. Create all objects here.
OrientationTestApp::OrientationTestApp() {
	addModifier(new poCamera2D(poColor::red));
    
    bg = new poRectShape(getWindowWidth(), getWindowHeight());
    bg->fillColor.set255(255,0,0);
    addChild(bg);
    addEvent(PO_TOUCH_BEGAN_EVENT, this);
    
    arrow = new poRectShape("resource/arrow-up.png");
    addChild(arrow);
    centerArrow();
    
    friction = 0.98;
    circle = new poOvalShape(50,50,100);
    circle->fillColor = poColor::grey;
    addChild(circle);
    
    addEvent(PO_ROTATION_EVENT, this);
    
    poStartAccelerometer(10.0f);
    addEvent(PO_ACCELEROMETER_EVENT, this);
    
    //Show Accel Info
    accelInfo = new poTextBox(150);
    accelInfo->setText("Info");
    accelInfo->setFont(poGetFont("resource/Arial.ttf"));
    accelInfo->setTextSize(16);
    accelInfo->textColor = poColor::white;
    accelInfo->useTextBoundsAsBounds(true);
    accelInfo->doLayout();
    addChild(accelInfo);
    
    bLockOrientation = false;
}

// APP DESTRUCTOR. Delete all objects here.
OrientationTestApp::~OrientationTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void OrientationTestApp::update() {
    
    //Add Friction
    accel *= friction;
    
    //Add acceleration to circle
    circle->position += accel;
    
    //Clamp to screen edges
    float minX = circle->getWidth()/2;
    float maxX = getWindowWidth() - circle->getWidth()/2;
    float minY = circle->getHeight()/2;
    float maxY = getWindowHeight() - circle->getHeight()/2;
    
    circle->position.x  = poClamp(minX, maxX, circle->position.x);
    circle->position.y  = poClamp(minY, maxY, circle->position.y);
    
    //If we're at the edge set accel to 0, z is always 0 since we're in 2D
    if(circle->position.x == minX || circle->position.x == maxX) accel.x = 0;
    if(circle->position.y == minY || circle->position.x == maxY) accel.y = 0;
    circle->position.z = 0.0f;
}

// DRAW. Called once per frame. Draw objects here.
void OrientationTestApp::draw() {
}

//Tween arrow to center after orientation change
void OrientationTestApp::centerArrow() {
	poPoint newPos(getWindowWidth()/2 - arrow->getWidth()/2,  getWindowHeight()/2 - arrow->getHeight()/2, 0);
    arrow->positionTween
    .set(newPos)
    .setDuration(0.5f)
    .setDelay(0.1f)
    .setTweenFunction(PO_TWEEN_QUAD_OUT_FUNC)
    .start();
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void OrientationTestApp::eventHandler(poEvent *event) {
    switch (event->type) {
        case PO_TOUCH_BEGAN_EVENT:
            if(bLockOrientation) {
                poSetAutoRotateOrientations(PO_VERTICAL | PO_HORIZONTAL);
                bg->fillColor.set255(255,0,0);
            } else {
                poSetAutoRotateOrientations(poGetOrientation());
                bg->fillColor.set255(155,0,0);
            }
            
            bLockOrientation = !bLockOrientation;
            break;
            
        case PO_ROTATION_EVENT: {
            bg->reshape(getWindowWidth(), getWindowHeight());
            switch (poGetOrientation()) {
                case PO_HORIZONTAL_LEFT:
                    std::cout << "Left" << std::endl;
                    break;
                case PO_HORIZONTAL_RIGHT:
                    std::cout << "Right" << std::endl;
                    break;
                case PO_VERTICAL_UP:
                    std::cout << "Up" << std::endl;
                    break;
                case PO_VERTICAL_DOWN:
                    std::cout << "Down" << std::endl;
                    break;
            }
            
            centerArrow();
            break;
        }
            
        case PO_ACCELEROMETER_EVENT:
            //Add motion to accelerometer
            accel += event->motion;
            
            //Update Accel info text
            accelInfo->setText("Accelerometer:\nX: " + poToString(event->motion.x)  + "\nY: " + poToString(event->motion.y));
            accelInfo->doLayout();
            accelInfo->position.set(getWindowWidth()/2 - accelInfo->getWidth()/2, getWindowHeight()-accelInfo->getHeight(), 0.0f);
            break;
    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void OrientationTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
