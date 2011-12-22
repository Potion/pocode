/*
 *  poVisionTrackingSystem.cpp
 *  MOS_BASE_APP
 *
 *  Created by Jared Schiffman on 7/5/10.
 *  Copyright 2010 Potion. All rights reserved.
 *
 */

#include "poVisionTrackingSystem.h"



poVisionTrackingSystem::poVisionTrackingSystem( int vWidth, int vHeight, int camID )
{
    
	printf("========== making poVisionTrackingSystem ============\n");

    // establish video basics
    videoWidth = vWidth;
    videoHeight = vHeight;
    cameraID = camID;  
    
    // pre-set variables
    accumRejectionRange = 255;
    accumUpdateSpeed = 0.0001;
    float threshold = 25;
    gridBoxSize = 10;
    referencePoint.set( vWidth/2,vHeight/2,0 );
    previousChecksum = 0;
    overrideCheckSum = false;
    
    // construct raw image and mask image
    rawImage = new poVisionImageBuffer( vWidth, vHeight );
    maskImage = new poVisionImageBuffer( vWidth, vHeight );
    maskImage->setAllPixels( 255 );

    
    
	// 0. mask proc
	maskProc = new poVisionAndProc( vWidth, vHeight );	
	maskImage = new poVisionImageBuffer( vWidth, vHeight, "local/masks/mask_cam0.raw" );	
	for( int i=0; i<vWidth*vHeight; i++ )
		maskImage->image[i] = ( maskImage->image[i] > 128 ) ? 255 : 0;
	
    //maskImage->setAllPixels(255);
	// 1. blur proc
	blurProc = new poVisionBlurProc( vWidth, vHeight );
	blurProc->blurRepeat = 1;
	// 2. accum proc
	accumProc = new poVisionAccumulateProc( vWidth, vHeight );
	accumProc->rejectionRange = accumRejectionRange;
	accumProc->updateSpeed = accumUpdateSpeed;
    accumProc->doFullCapture = true;
	// 3. difference proc
	diffProc = new poVisionDifererenceProc( vWidth, vHeight );
	diffProc->bias = 0;
	// 4. threshhold proc
	threshProc = new poVisionThresholdProc( vWidth, vHeight );
	threshProc->threshold = threshold;

    
	// make background image and load from disk
	backgroundImage = new poVisionImageBuffer( vWidth, vHeight );
	backgroundImage->setAllPixels(0);
	//readBackground();

	// this is the grid algorithm and the previous grid algorithm
	gridAlgorithm = new poVisionGridAlgorithm( vWidth, vHeight, gridBoxSize );
	gridAlgorithm->referencePoint = referencePoint;
	previousGridAlgorithm = new poVisionGridAlgorithm( vWidth, vHeight, gridBoxSize );	
	previousGridAlgorithm->referencePoint = referencePoint;
	
    
	/*shapeTrackTB = new poTextBox( "", 200,50, poColor(0,1,0) ); //, poColor::white, 1, poColor::green );
	shapeTrackTB->setFont( poFontStore::get("fonts/AveniBla.ttf",15));
	shapeTrackTB->doLayout();
	shapeTrackTB->visible = false;*/
	
	frameCounter = 0;
}


void poVisionTrackingSystem::capture( unsigned char* image )
{
    rawImage->copy( image );
}

void poVisionTrackingSystem::capture( poVisionImageBuffer* image )
{
    rawImage->copy( image );
}


bool poVisionTrackingSystem::process()
{
	if ( frameCounter < 30 )
	{
		frameCounter++;
		if ( frameCounter == 29 )
			resetBuffers();
		return true;
	}
	
	// confirm that frame is new and do processing
	int newChecksum = rawImage->calculateChecksum();
	if ( newChecksum == previousChecksum || overrideCheckSum )
        return false;
    
    previousChecksum = newChecksum;
    
    // blur the image and mask it first
    blurProc->process( rawImage );
    maskProc->process( blurProc->outputBuffer, maskImage );
    
    // accumulate into background buffer
    accumProc->processProcOutput( maskProc );
    
    // subtract background buffer from foreground
    diffProc->process( maskProc->outputBuffer, accumProc->outputBuffer ); //backgroundImage );
    
    //threshold and push into grid algorithm
    threshProc->processProcOutput( diffProc);
    
    // process using the grid algorithm, swap two grid algorithms first
    poVisionGridAlgorithm* tmpGridAlgo = gridAlgorithm;
    gridAlgorithm = previousGridAlgorithm;
    previousGridAlgorithm = tmpGridAlgo;
    gridAlgorithm->process( threshProc->outputBuffer, rawImage );
    
    // do shape tracking
    trackShapes();
    
    return true;
}


void poVisionTrackingSystem::draw()
{
	
    float S = 1.0;
    
    // row 1
    rawImage->draw( 1,20, S );
    maskProc->outputBuffer->draw( 400, 20, S );
    accumProc->outputBuffer->draw( 800, 20, S );

    // row 2
	diffProc->outputBuffer->draw( 1, 500, S );
    threshProc->outputBuffer->draw( 400, 500, S );
    maskProc->outputBuffer->draw( 800, 500, S );
	gridAlgorithm->draw( 800,500, S );
	
	rawImage->draw( 1,1,1 );
	
	// draw frame
	/*
    int x = (int) DrawX;
    int y = (int) DrawY;
    glColor3f( 1,1,1 );
	glRectf(x,y,x+1400,y+240);
	
	float drawScale = 1.0;
	
	// draw background image
	backgroundImage->draw( x,y-320,drawScale );

	//draw raw incoming frame:
	rawImage->draw( x,y,drawScale);

	//draw mask:
	maskProc->outputBuffer->draw( x+330*1,y, drawScale );
	
	//draw threshold output and points
	threshProc->outputBuffer->draw( x+330*2,y, drawScale );
	gridAlgorithm->draw( x+330*2,y, false );
	
	//draw grid on top of raw frame
	rawImage->draw( x+330*3,y,drawScale );
	gridAlgorithm->drawSHAPES( x+330*3,y, true, objectMinBoxSize);
	//previousGridAlgorithm->drawSHAPES( x+330*3,y+300, true, objectMinBoxSize);*/
	
	// draw id on each shape (this moves a single text box around, drawing once for each shape)
	/*if ( shapeTrackTB->visible )
	{
		for( int i=0; i<gridAlgorithm->shapeCount; i++ )
		{
			//continue;
			gridShape* curShape = &gridAlgorithm->shapeList[i];
			char id_str[64];
			sprintf( id_str, "ID=%d,pix=%d\nage=%d\ndist=%.1f", curShape->trackedShapeID,
			curShape->boxCount, curShape->trackedAge, curShape->averageVelocity.length() );
			shapeTrackTB->setText( id_str );
			shapeTrackTB->doLayout();
			int tbX = x+330*3 + curShape->topPoint.x;
			int tbY = y + (rawImage->height - curShape->topPoint.y) - 10;
			tbY -= 10*(curShape->trackedShapeID%3);
			shapeTrackTB->position.set( tbX, tbY );
			shapeTrackTB->drawTree();
		}
	}*/
}


void	poVisionTrackingSystem::resetBuffers()
{
	accumProc->doFullCapture = true;	
	//writeBackground();
	//readBackground();
}


void	poVisionTrackingSystem::trackShapes()
{
	
	poPoint veryFarPoint(99999,99999,0);
	
	// set trackedForward to false for all previous shapes
	for( int j=0; j<previousGridAlgorithm->shapeCount; j++ )
		previousGridAlgorithm->shapeList[j].trackedForward = false;
	
	// set current shape ID to -1
	for( int i=0; i<gridAlgorithm->shapeCount; i++ )
	{
		gridAlgorithm->shapeList[i].trackedShapeID = -1;
		gridAlgorithm->shapeList[i].trackedAge = 0;
        gridAlgorithm->shapeList[i].lastClickTime = 0;
		gridAlgorithm->shapeList[i].lastClickPoint = veryFarPoint;
	}
	
	// compare all current shapes to previous shapes
	// look for closest shapes within distance (expanding set of 3 distances)
	int	maxTravelDistanceArray[3] = { 5, 20, 60 };
	for( int r=0; r<3; r++ )
	{
		int	maxTravelDistance = maxTravelDistanceArray[r];		// expanding max travel distance
		
		// for all current shapes
		for( int i=0; i<gridAlgorithm->shapeCount; i++ )
		{
			gridShape* curShape = &gridAlgorithm->shapeList[i];
			gridShape* bestMatchShape = NULL;
			
			float bestMatchDist = 999999999;
			// for all previous shapes
			for( int j=0; j<previousGridAlgorithm->shapeCount; j++ )
			{
				gridShape* prevShape = &previousGridAlgorithm->shapeList[j];
				poPoint diff = curShape->midPoint - prevShape->midPoint;
				float distanceSquared = diff.lengthSquared();
				// if distance is less than before and less than max allowable distance
				if ( distanceSquared < maxTravelDistance*maxTravelDistance )
				{
					if ( distanceSquared < bestMatchDist )
					{
						bestMatchShape = prevShape;
						bestMatchDist = distanceSquared;
					}
				}
			}
			
			// set ID to that of best matching shape
			if ( bestMatchShape != NULL )
			{
				curShape->trackedShapeID = bestMatchShape->trackedShapeID;
				curShape->trackedAge = bestMatchShape->trackedAge+1;
                curShape->lastClickTime = bestMatchShape->lastClickTime;
				curShape->lastClickPoint = bestMatchShape->lastClickPoint;
				curShape->immediateVelocity = curShape->closestPoint - bestMatchShape->closestPoint;
				curShape->averageVelocity += curShape->immediateVelocity;
				curShape->averageVelocity *= 0.9;//0.98;
				bestMatchShape->trackedForward = true;
			}
		}
	}
	
	// for all current shapes with no ID's, generate new ID
	for( int i=0; i<gridAlgorithm->shapeCount; i++ )
	{
		gridShape* curShape = &gridAlgorithm->shapeList[i];
		if ( curShape->trackedShapeID == -1 )
		{
			gridShape::trackedID_Generator++;
			curShape->trackedShapeID = gridShape::trackedID_Generator;
			curShape->trackedAge = 0;
            curShape->lastClickTime = 0;
			curShape->lastClickPoint = veryFarPoint;
			curShape->immediateVelocity.set(0,0,0);
			curShape->averageVelocity.set(0,0,0);
		}
	}
	
	// for all previous shapes that didn't get tracked forward (lost shapes)
    // 
	for( int j=0; j<previousGridAlgorithm->shapeCount; j++ )
	{
		gridShape* prevShape = &previousGridAlgorithm->shapeList[j];
        // only for shapes that didn't get tracked forward
		if ( prevShape->trackedForward )
			continue;
		//printf("lost shape %d!!!\n", prevShape->trackedShapeID );
		
		if ( gridAlgorithm->shapeCount+1 >= MAX_GRID_SHAPES )
			break;
		
		// try adding old shape to new grid algorithm
        // this only takes boxes that are in old grid and new grid
		gridAlgorithm->shapeCount++;
		gridShape* newShape = &gridAlgorithm->shapeList[ gridAlgorithm->shapeCount-1 ];
		newShape->trackedShapeID = prevShape->trackedShapeID;
		newShape->trackedAge = prevShape->trackedAge+1;
        newShape->lastClickTime = prevShape->lastClickTime;
		newShape->lastClickPoint = prevShape->lastClickPoint;
		newShape->immediateVelocity = prevShape->immediateVelocity; // newShape->midPoint - prevShape->midPoint;
		newShape->averageVelocity += newShape->immediateVelocity;
		newShape->averageVelocity *= 0.98;
		
		// make "new" shape and copy over box list from old to new
		int newBoxCount = 0;
		for( int n=0; n<prevShape->boxCount; n++ )
		{
			int indX = prevShape->boxList[n]->indX;
			int indY = prevShape->boxList[n]->indY;
			gridBox* box = &gridAlgorithm->theGrid[indX][indY];
			if ( box->isFull )		// box must exist in new grid to be set
			{
				newShape->boxList[newBoxCount] = box;
				newBoxCount++;
				
				// remove from other shape
				if  ( box->ownerShape != NULL )
					box->ownerShape->removeBox( box );
				box->ownerShape = newShape;
			}
		}
		
		newShape->boxCount = newBoxCount;
		//printf("new shape %d box count: %d\n", newShape->trackedShapeID, newBoxCount );
		gridAlgorithm->findBestPointInShape( newShape, rawImage );
	}
	
	//if any shape has the same id as another, change it
	for( int i=0; i<gridAlgorithm->shapeCount; i++ )
	{
		gridShape* curShape = &gridAlgorithm->shapeList[i];
		for( int f=0; f<gridAlgorithm->shapeCount; f++ )
		{
			if( f==i )
                continue; //make sure not the same
			if( curShape->trackedShapeID == gridAlgorithm->shapeList[f].trackedShapeID )
			{
				gridShape::trackedID_Generator++;
				curShape->trackedShapeID = gridShape::trackedID_Generator;
				break;
			}
		}
	}

	// re-findBestPoint in all shapes
	for( int i=0; i<gridAlgorithm->shapeCount; i++ )
	{
		gridShape* curShape = &gridAlgorithm->shapeList[i];
		gridAlgorithm->findBestPointInShape( curShape, rawImage );
		//gridAlgorithm->calculateContour( curShape );
		gridAlgorithm->calculateDetailedContour( curShape,threshProc->outputBuffer );
	}
}

bool	poVisionTrackingSystem::writeBackground()
{
	printf("writing background image\n");
	return maskProc->outputBuffer->write( "local/background"  );
}

bool	poVisionTrackingSystem::readBackground()
{
	printf("reading background image\n");
	return backgroundImage->read("local/background");
}


