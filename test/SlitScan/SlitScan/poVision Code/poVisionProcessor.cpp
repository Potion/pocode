//
//  poVisionProcessor.cpp
//  StarShape
//
//  Created by Jared Schiffman on 7/31/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poVisionProcessor.h"

poVisionProcessor::poVisionProcessor( int vWidth, int vHeight, int camID )
{
    // establish video basics
    videoWidth = vWidth;
    videoHeight = vHeight;
    cameraID = camID;

    // pre-set variables
    accumRejectionRange = 255;
    accumUpdateSpeed = 0.0001;
    float threshold = 20;
    gridBoxSize = 10;
    referencePoint.set( vWidth/2,vHeight/2,0 );
    
    // construct raw image and mask image
    rawImage = new poVisionImageBuffer( vWidth, vHeight );
    maskImage = new poVisionImageBuffer( vWidth, vHeight );
    maskImage->setAllPixels( 255 );
    
    // make image processors
    maskProc = new poVisionAndProc( vWidth, vHeight );

    blurProc = new poVisionBlurProc( vWidth, vHeight );
	blurProc->blurRepeat = 1;
    
    accumProc = new poVisionAccumulateProc( vWidth, vHeight );
	accumProc->rejectionRange = accumRejectionRange;
	accumProc->updateSpeed = accumUpdateSpeed;

    diffProc = new poVisionDifererenceProc( vWidth, vHeight );
	
    threshProc = new poVisionThresholdProc( vWidth, vHeight );
    threshProc->threshold = threshold;
    
    // make grid system
    gridSystem = new poVisionGridAlgorithm( vWidth, vHeight, gridBoxSize );
	gridSystem->referencePoint = referencePoint;

}

void    poVisionProcessor::capture( unsigned char* image )
{
    rawImage->copy( image );
}

void    poVisionProcessor::capture( poVisionImageBuffer* image )
{
    rawImage->copy( image );
}

bool    poVisionProcessor::process()
{
    int newCheckSum = rawImage->calculateChecksum();
    if ( lastCheckSum == newCheckSum )
        return false;
    
    // blur the image and mask it first
    blurProc->process( rawImage );
    maskProc->process( blurProc->outputBuffer, maskImage );
    
    // accumulate into background buffer
    accumProc->processProcOutput( maskProc );
    
    // subtract background buffer from foreground
    diffProc->processProcOutput( maskProc, accumProc );
    
    //threshold image
    threshProc->processProcOutput( diffProc );
    
    // do grid algorithm
    gridSystem->process( threshProc->outputBuffer, rawImage );
    
    return true;
}


void    poVisionProcessor::draw()
{
    float S = 0.5;
    
    // row 1
    rawImage->draw( 1,20, 0.5 );
    blurProc->outputBuffer->draw( 400, 20, S );
    maskProc->outputBuffer->draw( 800, 20, S );

    // row 2
    accumProc->outputBuffer->draw( 1, 300, S );
    diffProc->outputBuffer->draw( 400, 300, S );
    threshProc->outputBuffer->draw( 800, 300, S );
    gridSystem->draw( 800,300, 0.5 );

}


