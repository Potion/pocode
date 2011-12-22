//
//  poVisionEventGenerator.cpp
//  StarShape
//
//  Created by Jared Schiffman on 8/2/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include <iostream>
#include "poVisionConnection.h"
#include "poApplication.h"

poVisionConnection::poVisionConnection()
{
    theCalibrator = new meshCalibrator( 3,3 );
	theCalibrator->setupDestMesh( 400,400,1920-400,1920-400 );
	theCalibrator->read();
	
	cursorShape = new poOvalShape( 30,30, 30 );
	cursorShape->fillColor = poColor::red;
	
	minSpeedForClicking = 0.25;
	repeatClickTimingInSeconds = 1.0;
	minAgeInFrames = 3.0;

	showString = false;
}

void    poVisionConnection::process( poVisionTrackingSystem* VT )
{
	visionTracker = VT;
	
    poWindow* win = applicationCurrentWindow();
    
	pointList.clear();
	
    for( int i=0; i<visionTracker->getNumShapes(); i++ )
    {
        gridShape* shape = visionTracker->getShape(i);
        poPoint P = shape->closestPoint;
		
		if ( abs(P.x) < 0.01 && abs (P.y) < 0.01 )
			continue;
		
		// calibrate point
        poPoint PC;
		theCalibrator->processPoint( P, PC );
        
		// add to pointList
		pointList.push_back( PC );

		// adjust for FBO		
		PC.y += 240;

		// touchMove event 
		win->touchMove( PC.x, PC.y, shape->trackedShapeID );
		    
        // if point is stopped and last click time is more than 3 seconds ago
		if ( shape->immediateVelocity.length() > minSpeedForClicking )
			shape->lastClickTime = 0;
		
		poPoint vecToLastClick = PC - shape->lastClickPoint;
		
        if ( shape->immediateVelocity.length() < minSpeedForClicking && 
             time(0) - shape->lastClickTime >= repeatClickTimingInSeconds &&
			 vecToLastClick.length() > 5 )
		{
			// touchBegin event
            win->touchBegin( PC.x, PC.y, shape->trackedShapeID );
            shape->lastClickTime = time(0);
			shape->lastClickPoint = PC;
        }
    }
}



void	poVisionConnection::draw()
{
	//theCalibrator->drawSrc(0,0,1);
	//theCalibrator->drawDest(0,0,1);
	//theCalibrator->drawCalibration();
	
	poFont* font = getFont("Lucida Grande", 20.0 );
	
	for( int i=0; i<pointList.size(); i++ )
	{
		// this draws the red cursor			
		cursorShape->position = pointList[i];
		cursorShape->_drawTree();
		
		gridShape* shape = visionTracker->getShape(i);
		char str[32];
		sprintf( str, "%.2f (%d) (%d)", shape->immediateVelocity.length(), shape->trackedShapeID, (int)(time(0)-shape->lastClickTime) );
		
		if(showString){
			glColor3f( 1,1,1 );
			drawString( str, font, cursorShape->position+poPoint(20,20) );
		}
	}
}

