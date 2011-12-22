//
//  poVisionProcessor.h
//  StarShape
//
//  Created by Jared Schiffman on 7/31/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_VISION_PROCESSOR_H
#define PO_VISION_PROCESSOR_H

#include "poObject.h"
#include "poVisionImageProcs.h"
#include "poVisionGridAlgorithm.h"

class poVisionProcessor : public poObject
{
public:
    poVisionProcessor( int vWidth, int vHeight, int camID );
    
    void         capture( unsigned char* image );
    void         capture( poVisionImageBuffer* image );
    bool         process();
    
    virtual void update() { };
    virtual void draw();
    virtual void eventHandler( poEvent* E ) { };
    
    poVisionImageBuffer*            rawImage;
    poVisionImageBuffer*            maskImage;
    int                             videoWidth, videoHeight, cameraID;
    int                             lastCheckSum;
    
    poVisionAndProc*				maskProc;
	poVisionBlurProc*				blurProc;
	poVisionAccumulateProc*			accumProc;
	poVisionDifererenceProc*		diffProc;
	poVisionThresholdProc*			threshProc;
    
    poVisionGridAlgorithm*          gridSystem;
    
    float                           accumRejectionRange;
    float                           accumUpdateSpeed;
    int                             gridBoxSize;
    poPoint                         referencePoint;
};


#endif
