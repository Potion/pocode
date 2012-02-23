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
    
    poSetAutoRotateOrientations(PO_HORIZONTAL);
    
    arrow = new poRectShape("resource/arrow-up.png");
    addChild(arrow);
    
    friction = 0.98;
    circle = new poOvalShape(50,50,100);
    circle->fillColor = poColor::grey;
    addChild(circle);
    
    poSetAutoRotateOrientations(PO_VERTICAL);
    addEvent(PO_ROTATION_EVENT, this);
    
    poStartAccelerometer(10.0f);
    addEvent(PO_ACCELEROMETER_EVENT, this);
    
    accelInfo = new poTextBox(150);
    accelInfo->setText("Info");
    accelInfo->setFont(poGetFont("resource/Arial.ttf"));
    accelInfo->setTextSize(18);
    accelInfo->textColor = poColor::white;
    accelInfo->useTextBoundsAsBounds(true);
    accelInfo->doLayout();
    addChild(accelInfo);
    
    centerArrow();
}

// APP DESTRUCTOR. Delete all objects here.
OrientationTestApp::~OrientationTestApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void OrientationTestApp::update() {
    accelInfo->setText(poToString(accel));
    accelInfo->doLayout();
    accelInfo->position.set(getWindowWidth()/2 - accelInfo->getWidth()/2, getWindowHeight()-accelInfo->getHeight(), 0.0f);
    
    //accel *= friction;
    circle->position += accel;
    
    circle->position.x  = poClamp(circle->getWidth()/2, getWindowWidth() - circle->getWidth()/2, circle->position.x);
    circle->position.y  = poClamp(circle->getHeight()/2, getWindowHeight() - circle->getHeight()/2, circle->position.y);
    circle->position.z = 0.0f;
}

// DRAW. Called once per frame. Draw objects here.
void OrientationTestApp::draw() {
	
}

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
        case PO_ROTATION_EVENT: {
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
            accel = event->motion * 10;
            break;
    }
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void OrientationTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
