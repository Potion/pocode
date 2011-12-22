/*
 *  poVisionImageProcs.cpp
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poVisionImageProcs.h"


// ======================== poVisionImageProc ======================== //


poVisionImageProc::poVisionImageProc( int width, int height )
{
	outputBuffer = new poVisionImageBuffer( width, height );
}


// ======================== poVisionBackSubProc ======================== //

poVisionBackSubProc::poVisionBackSubProc( int width, int height )  : poVisionImageProc(width,height)
{
	backBuffer = new poVisionImageBuffer( width, height );
	framesUntilGrab = 0;
}


void	poVisionBackSubProc::process( poVisionImageBuffer* inputBuffer )
{
	// capture background
	if ( framesUntilGrab==0 )
	{
		backBuffer->copy( inputBuffer );
		framesUntilGrab = -1;
	}
	if ( framesUntilGrab > 0 )
		framesUntilGrab--;
	
	
	// subtract background buffer
	int imageSize = inputBuffer->width * inputBuffer->height;
	for( int n=0; n<imageSize; n++ )
	{
		int D = inputBuffer->getPixel(n) - backBuffer->getPixel(n);
		outputBuffer->setPixel( n, abs(D) );
	}	
}



// ======================== poVisionEdgeDetectProc ======================== //

poVisionEdgeDetectProc::poVisionEdgeDetectProc( int width, int height )  : poVisionImageProc(width,height)
{
	edgeReach = 1;
}


void	poVisionEdgeDetectProc::process( poVisionImageBuffer* inputBuffer )
{
	// do edge detection
	int E = edgeReach;
	for( int i=E; i<inputBuffer->width-E; i++ )
	{
		for( int j=E; j<inputBuffer->height-E; j++ )
		{
			int TL = inputBuffer->getPixel(i-E,j+E);
			int T = inputBuffer->getPixel(i,j+E);
			int TR = inputBuffer->getPixel(i+E,j+E);
			
			int L = inputBuffer->getPixel(i-E,j);
			int R = inputBuffer->getPixel(i+E,j);
			
			int BL = inputBuffer->getPixel(i-E,j-E);
			int B = inputBuffer->getPixel(i,j-E);
			int BR = inputBuffer->getPixel(i+E,j-E);
			
			int horiEdge = (TL+2*L+BL) - (TR+2*R+BR);
			int vertEdge = (TL+2*T+TR) - (BR+2*B+BR);
			int bothEdges = abs(horiEdge) + abs(vertEdge);
			
			outputBuffer->setPixel( i,j, bothEdges );
		}
	}
}


// ======================== poVisionAccumulateProc ======================== //

poVisionAccumulateProc::poVisionAccumulateProc( int width, int height ) : poVisionImageProc(width,height)
{
	doFullCapture = true;
	updateSpeed = 0.01;
	
	doAssymmetricUpdate = false;
	lighterUpdateSpeed = 0.10;
	darkerUpdateSpeed = 0.01;


	floatBuffer = (float*) malloc( sizeof(float)*width*height );
	memset( floatBuffer, 0, sizeof(float)*width*height );

	rejectionRange = 10;	// this should generally be less than than background sibtraction threshold
}

void	poVisionAccumulateProc::process( poVisionImageBuffer* inputBuffer )
{
	// do full capture when required
	if ( doFullCapture )
	{
		int imageSize = inputBuffer->width * inputBuffer->height;
		for( int n=0; n<imageSize; n++ )
			floatBuffer[n] = inputBuffer->getPixel(n);
		doFullCapture = false;
	}

	float differenceSum = 0;

	// accumulate buffer
	int imageSize = inputBuffer->width * inputBuffer->height;
	for( int n=0; n<imageSize; n++ )
	{
		if ( doAssymmetricUpdate==false )			// normal mode
		{
			float diff = inputBuffer->getPixel(n) - outputBuffer->getPixel(n);
			differenceSum += abs(diff);
			float V = inputBuffer->getPixel(n);
			if ( diff < rejectionRange )
				floatBuffer[n] += ( V - floatBuffer[n] ) * updateSpeed;
		}
		else										// assymmetric mode, darker and lighter change at different speeds
		{
			float diff = inputBuffer->getPixel(n) - outputBuffer->getPixel(n);
			differenceSum += abs(diff);
			float V = inputBuffer->getPixel(n);
			if ( abs(diff) < rejectionRange )
			{
				if ( diff > 0 )
					floatBuffer[n] += ( V - floatBuffer[n] ) * lighterUpdateSpeed;
				else
					floatBuffer[n] += ( V - floatBuffer[n] ) * darkerUpdateSpeed;
			}
		}

		outputBuffer->setPixel( n, floatBuffer[n] );
	}

	/*if ( differenceSum > inputBuffer->width * inputBuffer->height * 5 )
	{
		printf("resetting accumulation buffer %d\n", this);	
		doFullCapture = true;
	}*/
}



// ======================== poVisionDifererenceProc ======================== //

poVisionDifererenceProc::poVisionDifererenceProc( int width, int height ) : poVisionImageProc(width,height)
{
	bias = 0;
}

void	poVisionDifererenceProc::process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 )
{
	// accumulate buffer
	int imageSize = inputBuffer1->width * inputBuffer1->height;

	for( int n=0; n<imageSize; n++ )
	{
		int V = inputBuffer1->getPixel(n) - inputBuffer2->getPixel(n);
		if ( bias == 0 )
			V = abs(V);
		else if ( bias < 0 )					// only detect darker values
			V = ( V < 0 ) ? abs(V) : 0;
		else
			V = ( V > 0 ) ? abs(V) : 0;			// only detect lighter values
		outputBuffer->setPixel( n, V );
	}
}




// ======================== poVisionThresholdProc ======================== //

poVisionThresholdProc::poVisionThresholdProc( int width, int height ) : poVisionImageProc(width,height)
{
	threshold = 50;
}

void	poVisionThresholdProc::process( poVisionImageBuffer* inputBuffer )
{
	// threshold buffer into black and white
	int imageSize = inputBuffer->width * inputBuffer->height;
	for( int n=0; n<imageSize; n++ )
	{
		if ( inputBuffer->getPixel(n) > threshold )
			outputBuffer->setPixel( n, 255 );
		else
			outputBuffer->setPixel( n, 0 );
	}
}


// ======================== poVisionThresholdProcMAP ======================== //

poVisionThresholdMAPProc::poVisionThresholdMAPProc( int width, int height ) : poVisionImageProc(width,height)
{
	thresholdMAX = 50;
	thresholdMIN = 25;
	thresholdDIFF = float(thresholdMAX-thresholdMIN);
	
	MapBufferACTIVE = MapBufferREFERENCE = NULL;
}

void poVisionThresholdMAPProc::setMINMAX( int MIN_, int MAX_)
{
	if(MIN_ > MAX_ || MIN_ < 0 || MAX_ < 0)
	{
		printf("ERROR in poVisionThresholdProcMAP: MIN and MAX have errors \n");
		return;
	}
	
	thresholdMAX = MAX_;
	thresholdMIN = MIN_;
	thresholdDIFF = float(thresholdMAX-thresholdMIN);
	
	printf("Threshold settings are min %d max %d diff %f\n", thresholdMIN, thresholdMAX, thresholdDIFF);
}

void poVisionThresholdMAPProc::setMap()
{
	int MapSize = MapBufferACTIVE->width * MapBufferACTIVE->height;
	for( int n=0; n<MapSize; n++ )
	{
		MapBufferACTIVE->setPixel( n, thresholdMIN + int( float(MapBufferREFERENCE->getPixel(n))/255.0 * thresholdDIFF) );
	}	
}

//Buffer1 is the input, Buffer2 is the Map
void	poVisionThresholdMAPProc::process(  poVisionImageBuffer* inputBuffer )
{	
	// threshold buffer into black and white
	int imageSize = inputBuffer->width * inputBuffer->height;
	int MapSize = MapBufferACTIVE->width * MapBufferACTIVE->height;
	if(imageSize != MapSize)
	{
		printf("ERROR in poVisionThresholdProcMAP: imagesize and Mapsize !=\n");
		return;
	}
	
	for( int n=0; n<imageSize; n++ )
	{
		if ( inputBuffer->getPixel(n) > MapBufferACTIVE->getPixel(n)  )
			outputBuffer->setPixel( n, 255 );
		else
			outputBuffer->setPixel( n, 0 );
	}
}

// ======================== poVisionErodeProc ======================== //

poVisionErodeProc::poVisionErodeProc( int width, int height ) : poVisionImageProc(width,height)
{
}

void	poVisionErodeProc::process( poVisionImageBuffer* inputBuffer )
{
	// initially, just copy whole buffer, then only erode certain pixels
	outputBuffer->copy( inputBuffer );
	
	// do erosion, all surrounding pixels must be on (white) for pixel to stay on (white)
	int E = 1;
	for( int i=E; i<inputBuffer->width-E; i++ )
	{
		for( int j=E; j<inputBuffer->height-E; j++ )
		{
			int C = inputBuffer->getPixel(i,j);
			
			// only process on (white, not off) pixels
			if ( C != 0 )
			{
				int TL = inputBuffer->getPixel(i-E,j+E);
				int T = inputBuffer->getPixel(i,j+E);
				int TR = inputBuffer->getPixel(i+E,j+E);
				
				int L = inputBuffer->getPixel(i-E,j);
				int R = inputBuffer->getPixel(i+E,j);
				
				int BL = inputBuffer->getPixel(i-E,j-E);
				int B = inputBuffer->getPixel(i,j-E);
				int BR = inputBuffer->getPixel(i+E,j-E);
				
				int surround = TL & T & TR & L & R & BL & B & BR;
				if ( surround == 0 )
					outputBuffer->setPixel( i,j, 0 );
			}
		}
	}
}




// ======================== poVisionDilateProc ======================== //

poVisionDilateProc::poVisionDilateProc( int width, int height ) : poVisionImageProc(width,height)
{
}

void	poVisionDilateProc::process( poVisionImageBuffer* inputBuffer )
{
	// initially, just copy whole buffer, then only erode certain pixels
	outputBuffer->copy( inputBuffer );
	
	// do dilation, if any surrounding pixels are on (white)  pixel becomes (white)
	int E = 1;
	for( int i=E; i<inputBuffer->width-E; i++ )
	{
		for( int j=E; j<inputBuffer->height-E; j++ )
		{
			int C = inputBuffer->getPixel(i,j);

			// only process off (black) pixels
			if ( C == 0 )
			{
				int TL = inputBuffer->getPixel(i-E,j+E);
				int T = inputBuffer->getPixel(i,j+E);
				int TR = inputBuffer->getPixel(i+E,j+E);
				
				int L = inputBuffer->getPixel(i-E,j);
				int R = inputBuffer->getPixel(i+E,j);
				
				int BL = inputBuffer->getPixel(i-E,j-E);
				int B = inputBuffer->getPixel(i,j-E);
				int BR = inputBuffer->getPixel(i+E,j-E);
				
				int surround = TL | T | TR | L | R | BL | B | BR;
				if ( surround )
					outputBuffer->setPixel( i,j, 255 );
			}
		}
	}
}



// ======================== poVisionBlurProc ======================== //

poVisionBlurProc::poVisionBlurProc( int width, int height )  : poVisionImageProc(width,height)
{
	tmpBuffer = new poVisionImageBuffer( width, height );
	blurRepeat = 1;
}


void	poVisionBlurProc::process( poVisionImageBuffer* inputBuffer )
{
	// initially, just copy whole buffer, then do progresssive blurring
	tmpBuffer->copy( inputBuffer );

	// repeat more than once if desired
	for( int r=0; r<blurRepeat; r++ )
	{
		
		// do blurring
		int E = 1;
		for( int i=E; i<tmpBuffer->width-E; i++ )
		{
			for( int j=E; j<tmpBuffer->height-E; j++ )
			{
				int TL = tmpBuffer->getPixel(i-E,j+E);
				int T = tmpBuffer->getPixel(i,j+E);
				int TR = tmpBuffer->getPixel(i+E,j+E);
				
				int L = tmpBuffer->getPixel(i-E,j);
				int C = tmpBuffer->getPixel(i,j);
				int R = tmpBuffer->getPixel(i+E,j);
				
				int BL = tmpBuffer->getPixel(i-E,j-E);
				int B = tmpBuffer->getPixel(i,j-E);
				int BR = tmpBuffer->getPixel(i+E,j-E);
				
				float blurSum = (TL+2*L+BL) + (T+2*C+B) + (TR+2*R+BR);
				blurSum /= 12.0;
				
				outputBuffer->setPixel( i,j, blurSum );
			}
		}
	
		// if blurring again, copy into tmpBuffer
		if ( r+1 < blurRepeat )
			tmpBuffer->copy( outputBuffer );
	}
}

// ======================== poVisionInvertProc ======================== //

poVisionInvertProc::poVisionInvertProc( int width, int height ) : poVisionImageProc(width,height)
{
}

void	poVisionInvertProc::process( poVisionImageBuffer* inputBuffer )
{
	// invert buffer
	int imageSize = inputBuffer->width * inputBuffer->height;
	for( int n=0; n<imageSize; n++ )
		outputBuffer->setPixel( n, 255-inputBuffer->getPixel(n) );
}


// ======================== poVisionAndProc ======================== //

poVisionAndProc::poVisionAndProc( int width, int height ) : poVisionImageProc(width,height)
{
}

void	poVisionAndProc::process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 )
{
	// accumulate buffer
	int imageSize = inputBuffer1->width * inputBuffer1->height;
	for( int n=0; n<imageSize; n++ )
		outputBuffer->setPixel( n, inputBuffer1->getPixel(n) & inputBuffer2->getPixel(n) );
}


// ======================== poVisionOrProc ======================== //

poVisionOrProc::poVisionOrProc( int width, int height ) : poVisionImageProc(width,height)
{
}

void	poVisionOrProc::process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 )
{
	// accumulate buffer
	int imageSize = inputBuffer1->width * inputBuffer1->height;
	for( int n=0; n<imageSize; n++ )
		outputBuffer->setPixel( n, inputBuffer1->getPixel(n) | inputBuffer2->getPixel(n) );
}


// ======================== poVisionConvolutionProc ======================== //

poVisionConvolutionProc::poVisionConvolutionProc( int width, int height )  : poVisionImageProc(width,height)
{
	doSumOfDifferences = false;
}


void	poVisionConvolutionProc::process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* convBuffer )
{
	int convOffsetX = convBuffer->width/2;
	int convOffsetY = convBuffer->height/2;
	
	
	// calculate convolution matrix sum
	int convMatrixTotal = 0;
	if ( doSumOfDifferences )
	{
		convMatrixTotal = convBuffer->width*convBuffer->height;
		inputBuffer1->setAllPixels(255);
	}
	else
	{
		for( int n=0; n<convBuffer->width*convBuffer->height; n++ )
			convMatrixTotal += convBuffer->getPixel(n);
		inputBuffer1->setAllPixels(0);
	}
	
	// main image loops
	for( int i=convOffsetX; i<inputBuffer1->width-convOffsetX; i++ )
	{
		for( int j=convOffsetY; j<inputBuffer1->height-convOffsetY; j++ )
		{
			
			int convSum = 0;
			
			// convolution loops
			for( int m=0; m<convBuffer->width; m++ )
			{	
				for( int n=0; n<convBuffer->height; n++ )
				{
					int x = i+m-convOffsetX;
					int y = j+n-convOffsetY;
					if ( x<0 || x>=inputBuffer1->width || y<0 || y>=inputBuffer1->height )
						continue;
					
					if ( doSumOfDifferences )
						convSum += abs( inputBuffer1->getPixel(x,y) - convBuffer->getPixel(m,n) );
					else
						convSum += inputBuffer1->getPixel(x,y) * convBuffer->getPixel(m,n);
						
				}
			}
			
			// divide by total
			if ( convMatrixTotal != 0 )
				convSum /= convMatrixTotal;
			
			outputBuffer->setPixel( i,j, convSum );
		}
	}
	

}


// ======================== poVisionBlurProc ======================== //

poVisionMOSnoiseProc::poVisionMOSnoiseProc( int width, int height )  : poVisionImageProc(width,height)
{

}


void	poVisionMOSnoiseProc::process( poVisionImageBuffer* inputBuffer )
{
	// do line elimination
	int E = 2;
	for( int i=0; i<inputBuffer->width; i++ )
	{
		for( int j=E*3; j<inputBuffer->height-E*3; j++ )
		{
			int T1 = inputBuffer->getPixel(i,j+E*1);
			int T2 = inputBuffer->getPixel(i,j+E*2);
			int T3 = inputBuffer->getPixel(i,j+E*3);
			int Tave = (T1+T2+T3) / 3;

			int C = inputBuffer->getPixel(i,j);
			
			int B1 = inputBuffer->getPixel(i,j-E*1);
			int B2 = inputBuffer->getPixel(i,j-E*2);
			int B3 = inputBuffer->getPixel(i,j-E*3);
			int Bave = (B1+B2+B3) / 3;


			// already on an edge, don't do anything
			if ( abs(Tave-Bave) > 10 ) 
			{
				outputBuffer->setPixel( i,j, C );
				continue;
			}

			// if in regular space, compare point to vertical average
			// if it's more than 10 away, make it same as average
			float ave = (T1+T2+T3+B1+B2+B3)/6;
			int diff = abs( ave - C );
			if ( diff > 10 )
				outputBuffer->setPixel( i,j, ave );
			else
				outputBuffer->setPixel( i,j, C );
		}
	}
}

