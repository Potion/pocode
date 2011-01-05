/*
 *  poSensing.cpp
 *  poFramework_static_lib
 *
 *  Created by Jared Schiffman on 9/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "poCameraConnection.h"



// ===================== poCameraConnection ===================== //


int					poCameraConnection::numCameras = 4;
int					poCameraConnection::PO_CAMERA_REFRESH_EVENT = -1;
poCameraConnection* poCameraConnection::instance = NULL;
bool				poCameraConnection::useNetwork = false;


poCameraConnection::poCameraConnection()
{
	rootNode = NULL;

 	cameraData = new SuperPack();
	for( int i=0; i<4; i++ )
		cameraData->cameraSet[i].numObjects = 0;
	numActivePoints = 0;

	fourCameraShapeSet = new poFourCameraShapeSet;
	for( int i=0; i<4; i++ )
		fourCameraShapeSet->cameraShapeSets[i].numShapes = 0;
	numActiveShapes = 0;

	freshData = false;
	if( useNetwork )
		boost::thread runner(ReceiverThread(9999, &freshData, cameraData));
	
	calibratorState = -1;
	for( int i=0; i<4; i++ )
		calibratorSet[i] = new poCameraCalibrator();			// these may be replaced with other calibrators
	
	freshData = false;
	doDrawing = false;

	if ( PO_CAMERA_REFRESH_EVENT == -1 )
		PO_CAMERA_REFRESH_EVENT = poEvent::registerNewEvent();
}


poCameraConnection* poCameraConnection::getInstance()
{
	if(instance == NULL)
		instance = new poCameraConnection();
	return instance;
}

void poCameraConnection::update()
{
	// during calibration, pass points to calibrator
	if ( calibratorState != -1 && calibratorState < numCameras )
	{
		doCalibrationSequence();
	}

	// normal operation, transform calibrated points
	if( calibratorState == -1 )
	{
		doPointTransformation();		// old method
		doShapeTransformation();		// new method
		
		launchEventCascade();

		freshData = false;
		return;
	}
}


void	poCameraConnection::doCalibrationSequence()
{
	datum* singleCamData = &cameraData->cameraSet[ calibratorState ];
	if ( singleCamData->numObjects==1 )
		calibratorSet[ calibratorState ]->takeSamplePoint( singleCamData->objects[0].tipX, singleCamData->objects[0].tipY );
	
	// when calibration is done, go to next calibrator or finish all calibration
	if ( calibratorSet[ calibratorState ]->isFinished() )
	{
		calibratorState++;
		printf("moving to next calibrator %d\n", calibratorState );
		if ( calibratorState==numCameras )
		{
			printf("PO_CAMERA_CONNECTION last camera calibrated\n");
			calibratorState = -1;
		}
		else
			calibratorSet[ calibratorState ]->reset();
	}
}


void	poCameraConnection::doPointTransformation()
{
	numActivePoints = 0;
	for( int c=0; c<numCameras; c++)
	{
		datum* singleCamData = &cameraData->cameraSet[c];
		for(int j=0; j<singleCamData->numObjects; j++)
		{
			poPoint untransformed = poPoint( singleCamData->objects[j].tipX, singleCamData->objects[j].tipY, 0 );   //modified by Xiaoyang
			poPoint transformed = calibratorSet[c]->transformPoint(untransformed);
			
			pointList[numActivePoints].x = transformed.x + originShiftPoint.x;
			pointList[numActivePoints].y = transformed.y + originShiftPoint.y;
			pointList[numActivePoints].handled = false;
			numActivePoints += 1;
			//printf("%.1f %.1f ==> %.1f %.1f\n \n ", untransformed.x, untransformed.y, transformed.x, transformed.y );
		}
	}
}


void  poCameraConnection::onePointTransformation( int calib, poSimplePoint2D &inPoint, poSimplePoint2D &outPoint )
{
	poPoint untransformed = poPoint( inPoint.x, inPoint.y );
	poPoint transformed = calibratorSet[calib]->transformPoint(untransformed);
	outPoint.set( transformed );
}


void	poCameraConnection::doShapeTransformation()
{
	numActiveShapes = 0;
	for( int c=0; c<numCameras; c++)
	{
		poCameraShapeSet* inShapeSet = &this->fourCameraShapeSet->cameraShapeSets[c];
		for(int j=0; j<inShapeSet->numShapes; j++)
		{
			poCameraShape* in_shape = &inShapeSet->shapes[j];
			poCameraShape* out_shape = &shapeList[ numActiveShapes ];

			for( int n=0; n<NUM_CAMERA_SHAPE_CONTOUR_POINTS; n++ )
				onePointTransformation( c, in_shape->contourPoints[n], out_shape->contourPoints[n] );
			
			onePointTransformation( c, in_shape->topPoint, out_shape->topPoint );
			onePointTransformation( c, in_shape->bottomPoint, out_shape->bottomPoint );
			onePointTransformation( c, in_shape->leftPoint, out_shape->leftPoint );
			onePointTransformation( c, in_shape->rightPoint, out_shape->rightPoint );
			onePointTransformation( c, in_shape->midPoint, out_shape->midPoint );
			onePointTransformation( c, in_shape->brightestPoint, out_shape->brightestPoint );
			onePointTransformation( c, in_shape->closestPoint, out_shape->closestPoint );
			onePointTransformation( c, in_shape->furthestPoint, out_shape->furthestPoint );

			out_shape->majorAxisAngle = in_shape->majorAxisAngle;
			out_shape->tipAxisAngle = in_shape->tipAxisAngle;
			out_shape->age = in_shape->age;
			out_shape->averageVelocity = in_shape->averageVelocity;
			out_shape->immediateVelocity = in_shape->immediateVelocity;
			out_shape->shapeID = in_shape->shapeID;
			out_shape->gridBoxCount = in_shape->gridBoxCount;

			numActiveShapes++;
		}
	}
}


void	poCameraConnection::launchEventCascade()
{
	// send PO_CAMERA_REFRESH_EVENT to all objects through rootNode
	poEvent e;
	e.type = PO_CAMERA_REFRESH_EVENT;
	e.key = 0;
	e.x = e.y = 0;
	e.modifiers = 0;
	e.message = NULL;
	
	if ( rootNode != NULL )
		rootNode->processEventTree(&e);
	else
		printf("WARNING poCameraConnection rootNode is NULL, call attachCameraEventsToAllObjects()\n");
}

void poCameraConnection::draw()
{
	if ( doDrawing == false )			// never set poCameraConnection->visible, since update will be disabled
		return;
	
	// draw untransformed points
	
	for( int c=0; c<numCameras; c++)		// draw raw camera points
	{
		datum* singleCamData;
		singleCamData = &cameraData->cameraSet[c];
		
		glPushMatrix();
//		glTranslatef( 300+350*c,300,0 );	
		glTranslatef( 321*c,300,0 );		//modified by Xiaoyang Feng
		glColor4f(0.5, 0.5, 0.5, 0.25);		// draw background camera rect for each camera
		if ( ! calibratorSet[c]->isFinished() && singleCamData->numObjects > 1 )
			glColor4f( 1,0,0,0.5 );
		glRectf( 0,0,320,240 );
		glColor3f(0, 1, 1);
		
		for(int j=0; j<singleCamData->numObjects; j++)
		{
			poPoint untransformed = poPoint( singleCamData->objects[j].tipX, singleCamData->objects[j].tipY, 0 );
			glRectf( untransformed.x-3, untransformed.y-3, untransformed.x+3, untransformed.y+3);
		}
		
		// draw captured calibrated points
		calibratorSet[c]->drawCapturedPoints();
		glPopMatrix();
	}
	
	
	glPushMatrix();
	glTranslatef( -originShiftPoint.x, -originShiftPoint.y, 0 );
	// draw transformed points
	glColor3f(1.f, 1.f, 0.f);
	for(int i=0; i<numActivePoints; i++)
		glRectf(pointList[i].x-5, pointList[i].y-5, pointList[i].x+5, pointList[i].y+5);
	glPopMatrix();
	
	// draw active calibrator only
	if ( calibratorState >=0 && calibratorState < numCameras )
		calibratorSet[ calibratorState ]->draw();
	

}

void	poCameraConnection::readCalibration()
{
	printf("reading calibration\n");
	calibratorState = -1;
	for( int i=0; i<numCameras; i++ )
	{
		printf("reading calibration %d\n", i );
		calibratorSet[i]->readCalibration();
	}
}

void	poCameraConnection::launchCalibrator()
{
	for( int i=0; i<numCameras; i++ )
		calibratorSet[i]->reset();
	calibratorState = 0;
	doDrawing = true;
}

void	poCameraConnection::attachCameraEventsToAllObjects( poObject* theRootNode )
{
	rootNode = theRootNode;
	poCameraEventProcessor::attachCameraEvents( rootNode );
}


// ===================== poCameraEventProcessor ===================== //

int		poCameraEventProcessor::FRAME_SKIP_LAG = 3;
int		poCameraEventProcessor::DEFAULT_NUM_FRAMES_UNTIL_EVENT = 15;
int		poCameraEventProcessor::CLICK_REPEAT_RATE = 1000;
bool	poCameraEventProcessor::ALLOW_HANDLED_POINTS = false;
bool	poCameraEventProcessor::CLICK_FIRE_ONLY_ONCE = true;

poCameraEventProcessor::poCameraEventProcessor( poObject* theFocusObj )
{
	focusObj = theFocusObj;

	pointsInsideFrameCounter = 0;
	noPointsInsideFrameCounter = 0;
	numFramesUntilEvent = DEFAULT_NUM_FRAMES_UNTIL_EVENT;
	mouseInsideLastFrame = false;
	pointInsideFader = 0;
}

void poCameraEventProcessor::process()
{
	if( !focusObj->visible || focusObj->alpha < 0.01 )
		return;
	
	bool	foundPointsInside = false;
	
	poCameraConnection* myCam = poCameraConnection::getInstance();
	
	//printf( "ACTIVE: %d\n", myCam->numActivePoints );
	// for all points
	float mX, mY;
	for(int i=0; i<myCam->numActivePoints; i++)
	{
		if( !ALLOW_HANDLED_POINTS )
			if ( myCam->pointList[i].handled )
				continue;
		
		mX = myCam->pointList[i].x;
		mY = myCam->pointList[i].y;		
		
		// if point is inside shape
		if( focusObj->pointInside( mX, mY ) )		
		{
			// set handled for point
			myCam->pointList[i].handled = true;
			
			// adjust counter variables
			pointsInsideFrameCounter += 1;
			noPointsInsideFrameCounter = 0;
			foundPointsInside = true;
	
			// if we have one point, break away
			break;
		}
	}
	
	// if no points inside shape
	if ( foundPointsInside == false )
	{
		noPointsInsideFrameCounter++;
		if ( noPointsInsideFrameCounter > FRAME_SKIP_LAG )
		{
			pointsInsideFrameCounter = 0;
		}
	}
	
	// repeat click after set time
	/*if ( pointsInsideFrameCounter > CLICK_REPEAT_RATE )
	{
		pointsInsideFrameCounter = 0;
	}*/
	
	
	// process and issue all events ==================
	
	// handle mouse over event
	/*for(int i=0; i<obj->events[PO_MOUSE_OVER_EVENT].size(); i++)
	{
		poEvent* event = obj->events[PO_MOUSE_OVER_EVENT][i];
		
		if ( foundPointsInside == true )
			event->receiver->eventHandler( event );
	}*/
	
	// handle enter event
	bool mouseCurrentInsideLocal = false;	// we need this variable in case it's set by enter, before leave gets to it
	if ( focusObj->hasEvents(PO_MOUSE_ENTER_EVENT) )
	{
		for(int i=0; i<focusObj->events[PO_MOUSE_ENTER_EVENT].size(); i++)
		{
			poEvent* event = focusObj->events[PO_MOUSE_ENTER_EVENT][i];
			
			mouseCurrentInsideLocal = event->receiver->mouseCurrentlyInside;
			
			if ( foundPointsInside == false )
				event->receiver->mouseCurrentlyInside = false;
			else
			{
				if ( mouseInsideLastFrame == false )
					event->receiver->eventHandler( event );
				event->receiver->mouseCurrentlyInside = true;
			}
		}
	}
	
	// handle leave event
	if ( focusObj->hasEvents(PO_MOUSE_LEAVE_EVENT) )
	{
		for(int i=0; i<focusObj->events[PO_MOUSE_LEAVE_EVENT].size(); i++)
		{
			poEvent* event = focusObj->events[PO_MOUSE_LEAVE_EVENT][i];
			if ( foundPointsInside == false )
			{
				if ( mouseInsideLastFrame == true )
					event->receiver->eventHandler( event );
				event->receiver->mouseCurrentlyInside = false;
			}
			else
				event->receiver->mouseCurrentlyInside = true;
		}
	}

	// handle mouse move event
	if ( focusObj->hasEvents(PO_MOUSE_MOVE_EVENT) )
	{
		for(int i=0; i<focusObj->events[PO_MOUSE_MOVE_EVENT].size(); i++)
		{
			poEvent* event = focusObj->events[PO_MOUSE_MOVE_EVENT][i];
			// send every point
			for(int n=0; n<myCam->numActivePoints; n++)
			{
				event->x = mX;
				event->y = mY;
				event->localX = event->x;
				event->localY = event->y;
				float tempZ = 0;
				event->receiver->globalToLocal( event->localX, event->localY,tempZ );
				event->receiver->eventHandler( event );
			}
		}
	}

	// handle mouse over event
	if ( focusObj->hasEvents(PO_MOUSE_OVER_EVENT) && foundPointsInside == true )
	{	
		//printf("mouse over\n");
		for(int i=0; i<focusObj->events[PO_MOUSE_OVER_EVENT].size(); i++)
		{
			poEvent* event = focusObj->events[PO_MOUSE_OVER_EVENT][i];
			event->x = mX; //yCam->pointList[n].x;
			event->y = mY; //myCam->pointList[n].y;
			event->localX = event->x;
			event->localY = event->y;
			float tempZ = 0;
			event->receiver->globalToLocal( event->localX, event->localY, tempZ );
			//poEvent* event = focusObj->events[PO_MOUSE_OVER_EVENT][i];
			event->receiver->eventHandler( event );
			//printf("here.....mouse over event\n");
		}

	}


	// handle mouse click event
	if ( focusObj->hasEvents(PO_MOUSE_PRESS_EVENT) )
	{
		//if ( pointsInsideFrameCounter == numFramesUntilEvent+1 )
		if ( ( CLICK_FIRE_ONLY_ONCE && pointsInsideFrameCounter == numFramesUntilEvent+1 ) ||
			  ( !CLICK_FIRE_ONLY_ONCE && pointsInsideFrameCounter >= numFramesUntilEvent+1 ) )
		{
				// this is to ensure that we don't fire twice if there
				// are no points in the next frame
				pointsInsideFrameCounter++;
				
				for(int i=0; i<focusObj->events[PO_MOUSE_PRESS_EVENT].size(); i++)
				{

					poEvent* event = focusObj->events[PO_MOUSE_PRESS_EVENT][i];
					event->x = mX; //yCam->pointList[n].x;
					event->y = mY; //myCam->pointList[n].y;
					event->localX = event->x;
					event->localY = event->y;
					float tempZ = 0;
					event->receiver->globalToLocal( event->localX, event->localY, tempZ );
					event->receiver->eventHandler( event );
				}
		}
	}
	
	mouseInsideLastFrame = foundPointsInside;
	
	return;
}


bool poCameraEventProcessor::eventHandler(poEvent* event)
{
	if ( event->type == poCameraConnection::PO_CAMERA_REFRESH_EVENT )
		process();
	return false;
}


void	poCameraEventProcessor::attachCameraEvents( poObject* obj )
{
	if ( obj->hasEvents( PO_MOUSE_PRESS_EVENT ) > 0 ||
		 obj->hasEvents( PO_MOUSE_ENTER_EVENT ) > 0 ||
		 obj->hasEvents( PO_MOUSE_LEAVE_EVENT ) > 0 ||
		 obj->hasEvents( PO_MOUSE_MOVE_EVENT ) > 0 ||
		 obj->hasEvents( PO_MOUSE_OVER_EVENT ) > 0 )
	{
		// this should already be registered, but just in case
		if ( poCameraConnection::PO_CAMERA_REFRESH_EVENT == -1 )
			poCameraConnection::PO_CAMERA_REFRESH_EVENT = poEvent::registerNewEvent();

		// obj->addModifier( new poCameraEventModifier() );
		// MEMORY LEAK: this new poCameraEventProcessor will not get deleted if the object is deleted
		// printf("adding event modifier to %s\n", obj->getName() );
		obj->addEvent( poCameraConnection::PO_CAMERA_REFRESH_EVENT, new poCameraEventProcessor(obj) );
	}
	
	for( int i=0; i<obj->numChildren(); i++ )
		attachCameraEvents( obj->getChild(i) );
}

void	poCameraEventProcessor::setFramesUntilMousePress( poObject* obj, int numFrames )
{
	for(int i=0; i<obj->events[poCameraConnection::PO_CAMERA_REFRESH_EVENT].size(); i++)
	{
		// get event
		poEvent* event = obj->events[poCameraConnection::PO_CAMERA_REFRESH_EVENT][i];
		// get receiver (poCameraEventProcessor) of event
		poCameraEventProcessor* eventProcessObj = (poCameraEventProcessor*) event->receiver;
		// set numFramesUntilEvent
		eventProcessObj->numFramesUntilEvent = numFrames;
	}
}



