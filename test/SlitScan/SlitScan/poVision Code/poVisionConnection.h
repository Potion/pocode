//
//  poVisionEventGenerator.h
//  StarShape
//
//  Created by Jared Schiffman on 8/2/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_VISION_CONNECTOR_H
#define PO_VISION_CONNECTOR_H

#include "poVisionTrackingSystem.h"
#include "poVisionCalibrator.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

class poCalibrator;

class poVisionConnection : public poObject
{
public:
    poVisionConnection();
    
    void				process( poVisionTrackingSystem* visionTracker );

    virtual void		draw();

	std::vector<poPoint>	pointList;
	
    meshCalibrator*			theCalibrator;
	poVisionTrackingSystem*	visionTracker;
	
	poOvalShape*			cursorShape;
	
	float minSpeedForClicking;
	float repeatClickTimingInSeconds;
	float minAgeInFrames;
	
	bool	showString;
};


#endif
