/*
 *  poVisionTrackingSystem.h
 *  MOS_BASE_APP
 *
 *  Created by Jared Schiffman on 7/5/10.
 *  Copyright 2010 Potion. All rights reserved.
 *
 */

#ifndef PO_VISION_TRACKING_SYSTEM
#define PO_VISION_TRACKING_SYSTEM

#include "poVisionSystem.h"

class poVisionTrackingSystem
{
public:
	poVisionTrackingSystem( int vWidth, int vHeight, int camID );
    
    void        capture( unsigned char* image );
    void        capture( poVisionImageBuffer* image );
    bool        process();
    
    void        draw();
	void        resetBuffers();
	void        trackShapes();

	bool        writeBackground();
	bool        readBackground();
	
	int								getNumShapes() { return gridAlgorithm->shapeCount; };
	gridShape*						getShape( int N ) { return &gridAlgorithm->shapeList[N]; };
	
    // basics
    poVisionImageBuffer*            rawImage;
    poVisionImageBuffer*            maskImage;
    poVisionImageBuffer*			backgroundImage;
    int                             videoWidth, videoHeight, cameraID;
    int                             previousChecksum;
    bool                            overrideCheckSum;
    
    // modifiers
    float                           accumRejectionRange;
    float                           accumUpdateSpeed;
    int                             gridBoxSize;
    poPoint                         referencePoint;
    
	// image processors
	poVisionAndProc*				maskProc;
	poVisionBlurProc*				blurProc;
	poVisionAccumulateProc*			accumProc;
	poVisionDifererenceProc*		diffProc;
	poVisionThresholdProc*			threshProc;
	
    poVisionGridAlgorithm*          gridAlgorithm;
	poVisionGridAlgorithm*			previousGridAlgorithm;
	
	int								frameCounter;

};

#endif


