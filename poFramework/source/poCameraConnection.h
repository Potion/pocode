/*
 *  poSensing.h
 *  poFramework_static_lib
 *
 *  Created by Jared Schiffman on 9/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __PO_CAMERA_CONNECTION__
#define __PO_CAMERA_CONNECTION__


#include "boost/thread/thread.hpp"
#include "poSocket.h"
#include "poPoint.h"
#include "poEvent.h"
#include "poObject.h"
#include "poObjectModifier.h"
#include "poCameraCalibrator.h"

const static int  OBJECTS_TO_SEND = 100;

// old data types =====================
struct PackedComponent
{
	int size;
	int tipX, tipY;
	int closestX, closestY;
	int furthestX, furthestY;
	int centroidX, centroidY;
	int leftX, leftY;
	int rightX, rightY;
	int topX, topY;
	int bottomX, bottomY;
	bool valid;
};

struct datum
{
	int numObjects;
	PackedComponent objects[OBJECTS_TO_SEND];
};

struct SuperPack
{
	datum cameraSet[4];
};

struct CameraPoint
{
	float x, y;
	bool handled;
};

// new data types =====================

#define NUM_CAMERA_SHAPE_CONTOUR_POINTS 30
#define MAX_SHAPES_PER_CAMERA 30

struct poSimplePoint2D
{
	float x,y;

	void set( poPoint P ) { x = P.x; y = P.y; };
};

struct poCameraShape
{
	int		gridBoxCount;
	int		shapeID;
	int		age;

	poSimplePoint2D	immediateVelocity;
	poSimplePoint2D	averageVelocity;

	poSimplePoint2D closestPoint;
	poSimplePoint2D furthestPoint;
	poSimplePoint2D midPoint;
	poSimplePoint2D brightestPoint;
	poSimplePoint2D leftPoint;
	poSimplePoint2D rightPoint;
	poSimplePoint2D topPoint;
	poSimplePoint2D bottomPoint;
	float			majorAxisAngle;
	float			tipAxisAngle;

	poSimplePoint2D	contourPoints[NUM_CAMERA_SHAPE_CONTOUR_POINTS];
};

struct poCameraShapeSet
{
	int				numShapes;
	poCameraShape	shapes[MAX_SHAPES_PER_CAMERA];
};

struct poFourCameraShapeSet
{
	poCameraShapeSet	cameraShapeSets[4];
};


// receiver thread

class ReceiverThread
{
	public:
		ReceiverThread(unsigned short receivingPort, bool* freshData, SuperPack* sharedData)
		: client(new UDPSocket(receivingPort)), freshData(freshData), sharedData(sharedData)
		{*freshData = false;}
		
		void operator()()
		{
			unsigned short port;
			std::string from;
			
			void* localData = malloc(sizeof(SuperPack));
			while(true)
			{
				try {
					client->recvFrom(localData, sizeof(SuperPack), from, port);
					memcpy(sharedData, localData, sizeof(SuperPack));
					*freshData = true;
				} catch(const SocketException& e) {
					printf("%s\n", e.what());
				}
			}
			free(localData);
		}
		
		UDPSocket* client;
		bool* freshData;
		SuperPack* sharedData;
};

// poCameraConnection

#define MAX_CAMERA_SHAPES	(MAX_SHAPES_PER_CAMERA*4)

class poCameraConnection : public poObject
{
public:
	poCameraConnection();
	
	static poCameraConnection* getInstance();
	
	virtual void update();
	virtual void draw();
	
	void						setCalibrator( int N, poCameraCalibrator* C ) { calibratorSet[N] = C; };
	void						readCalibration();
	void						launchCalibrator();
	void						attachCameraEventsToAllObjects( poObject* theRootNode );

	void						doCalibrationSequence();
	void						onePointTransformation( int calib, poSimplePoint2D &inPoint, poSimplePoint2D &outPoint );
	void						doPointTransformation();	// old method
	void						doShapeTransformation();	// new method
	void						launchEventCascade();
	
	SuperPack					*cameraData;				// old data type
	int							numActivePoints;
	CameraPoint					pointList[OBJECTS_TO_SEND*4];
	poPoint						originShiftPoint;

	poFourCameraShapeSet		*fourCameraShapeSet;		// new data type
	int							numActiveShapes;
	poCameraShape				shapeList[MAX_CAMERA_SHAPES];

	poCameraCalibrator			*calibratorSet[4];			// replace these with new calibrators
	int							calibratorState;			// -1 is calibration off, 0..3 turn each calibrator on
	
	bool						freshData;
	bool						doDrawing;

	poObject*					rootNode;					// sends refresh events directly
	
	static int					numCameras;
	static int					PO_CAMERA_REFRESH_EVENT;
	static poCameraConnection*	instance;
	static bool					useNetwork;
};


class poCameraEventProcessor : public poObject
{
public:
	poCameraEventProcessor( poObject* theFocusObj );
	
	virtual void process();
	virtual bool eventHandler(poEvent* event);
	
	poObject*		focusObj;
	int				pointsInsideFrameCounter;
	int				noPointsInsideFrameCounter;
	int				numFramesUntilEvent;
	bool			mouseInsideLastFrame;
	float			pointInsideFader;
	
	static int		FRAME_SKIP_LAG;
	static int		DEFAULT_NUM_FRAMES_UNTIL_EVENT;
	static int		CLICK_REPEAT_RATE;
	static bool		ALLOW_HANDLED_POINTS;
	static bool		CLICK_FIRE_ONLY_ONCE;

	static void	attachCameraEvents( poObject* obj );
	static void	setFramesUntilMousePress( poObject* obj, int numFrames );
};


#endif
