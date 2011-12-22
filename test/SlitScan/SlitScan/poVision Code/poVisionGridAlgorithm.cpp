
#include "poVisionGridAlgorithm.h"
#include <time.h>

// ================================== gridBox ================================== //

gridBox::gridBox()
{
	x = y = 0;
	isFull = false;
	percentFull = 0.0;
	pixelCount  = 0;
	pixelList = NULL;
	label = -1;
}

void gridBox::setup( int boxWidth, int boxHeight )
{
	pixelList = new poVisPoint [ boxWidth * boxHeight ];
	ownerShape = NULL;
}


// ================================== gridShape ================================== //

#define MAX_BOXES_PER_SHAPE 2000
int	gridShape::trackedID_Generator = 0;

gridShape::gridShape()
{
	boxCount = 0;
	boxList = new gridBox* [ MAX_BOXES_PER_SHAPE ];
	trackedShapeID = -1;
	trackedAge = 0;
	immediateVelocity.set(0,0,0);
	averageVelocity.set(0,0,0);;
}

gridBox*	gridShape::removeBox( gridBox* B )
{
	if ( boxCount <= 1 )
		return NULL;

	// find and then remove box
	for( int i=0; i<boxCount; i++ )
	{
		if ( boxList[i]==B )
		{
			boxList[i] = boxList[boxCount-1];
			boxCount--;
			return B;
		}
	}
	return NULL;
}

// ================================== poVisionGridAlgorithm ================================== //

poVisionGridAlgorithm::poVisionGridAlgorithm( int vWidth, int vHeight, int boxSize )
{
	// establish video size
	videoWidth = vWidth;
	videoHeight = vHeight;
	
	// establish grid setup
	boxWidth = boxHeight = boxSize;
	gridWidth = videoWidth / boxWidth;
	gridHeight = videoHeight / boxHeight;
	
	percentFullThreshhold = 0.05;

	// create grid boxes
	theGrid = new gridBox* [ gridWidth ];
	for( int i=0; i<gridWidth; i++ )
	{
		theGrid[i] = new gridBox [ gridHeight ];
		for( int j=0; j<gridHeight; j++ )
		{
			theGrid[i][j].setup( boxWidth, boxHeight );
			theGrid[i][j].x = i*boxWidth;
			theGrid[i][j].y = j*boxHeight;
			theGrid[i][j].percentFull = (float) rand() / (float) RAND_MAX;
			theGrid[i][j].pixelCount = 0;
			theGrid[i][j].indX = i;
			theGrid[i][j].indY = j;
		}
	}
	
	// create grid shapes
	shapeCount = 0;
	shapeList = new gridShape [ MAX_GRID_SHAPES ];

	referencePoint.set( videoWidth/2, videoHeight/2, 0 );
	referenceLine = videoHeight / 2;
}

poVisionGridAlgorithm::~poVisionGridAlgorithm(void)
{
}

void poVisionGridAlgorithm::draw( int posX, int posY, float scale )
{
	glDisable( GL_DEPTH_TEST );

	// position drawing in place
	glPushMatrix();

    glTranslatef( posX, posY, 0 );
	glScalef( scale, scale, 1 );
    
	// draw grid boxes
    glBegin( GL_QUADS );
    glColor4f( 0,1,0,0.25 );
    glLineWidth( 1 );
    for( int i=0; i<gridWidth; i++ )
    {
        for( int j=0; j<gridHeight; j++ )
        {
            gridBox* GB = &theGrid[i][j];
        
            // drag stroke around "full" boxes
            if ( GB->isFull )
            {                
                glVertex2f( GB->x, GB->y );
                glVertex2f( GB->x+boxWidth, GB->y );
                glVertex2f( GB->x+boxWidth, GB->y+boxHeight );
                glVertex2f( GB->x, GB->y+boxHeight );
            }
        }
    }
    glEnd();


	// draw extremal points
	for( int i=0; i<shapeCount; i++ )
	{
		gridShape* S = &shapeList[i];
		glColor3f( 1,0,0.5 );
		int R = 4;
		
		poPoint P;
		glColor3f( 1,0.0,0.0 );
		P = S->closestPoint;
		//P = S->midPoint;											/// THE CHOSEN POINT
		//P = S->brightestPoint;
		glRectf( P.x-R, P.y-R, P.x+R, P.y+R );

		//draw major axis angle
		/*float len = 10;
		glColor3f( 1,0.8,0.0 );
		glLineWidth( 2 );
		glBegin( GL_LINES );
		glVertex2f( S->midPoint.x, S->midPoint.y );
			glVertex2f( S->midPoint.x+len*cos(S->majorAxisAngle), S->midPoint.y+len*sin(S->majorAxisAngle) );
		glEnd();*/

	}

	// draw reference line
	//glColor3f( 1,1,1 );
	//glRectf( 0,referencePoint.y,videoWidth,referencePoint.y );
	
	// draw reference point
	glColor3f( 1,0.8,0.5 );
	glRectf( referencePoint.x-5, referencePoint.y-5, referencePoint.x+5, referencePoint.y+5  );
	
	glPopMatrix();
}


void poVisionGridAlgorithm::process( poVisionImageBuffer* image, poVisionImageBuffer* rawImage )
{
	// fill boxes with pixels and calculate percentFull and isFull
	doBoxFilling( image );

	// label all boxes, number of labels = number of shapes
	shapeCount = doBoxLabeling();

	// build shapes from labeled boxes
	buildShapeFromBoxes();

	// find best point in every shape
	for( int i=0; i<shapeCount; i++ )
	{
		findBestPointInShape( &shapeList[i], rawImage );
		//calculateContour( &shapeList[i] );
	}
}


void poVisionGridAlgorithm::doBoxFilling( poVisionImageBuffer* image )
{
	// clear grid boxes
	for( int i=0; i<gridWidth; i++ )
	{
		for( int j=0; j<gridHeight; j++ )
		{
			theGrid[i][j].pixelCount = 0;
			theGrid[i][j].ownerShape = NULL;
		}
	}

	// accumulate pixels into boxes
	for( int i=0; i<image->width; i++ )
	{
		for( int j=0; j<image->height; j++ )
		{
			unsigned char pixel = image->getPixel(i,j);
			if ( pixel )
			{
				int iBox = i / boxWidth;
				int jBox = j / boxHeight;

				int N = theGrid[iBox][jBox].pixelCount;
				theGrid[iBox][jBox].pixelList[ N ].x = i;
				theGrid[iBox][jBox].pixelList[ N ].y = j;
				theGrid[iBox][jBox].pixelCount++;
				
				if ( theGrid[iBox][jBox].pixelCount > boxWidth*boxHeight )
				{
					printf( "too many pixels in box %d\n", theGrid[iBox][jBox].pixelCount );
					continue;
				}
			}
		}
	}

	// calculate percentFull  and isFull
	for( int i=0; i<gridWidth; i++ )
	{
		for( int j=0; j<gridHeight; j++ )
		{
			theGrid[i][j].percentFull = (float) theGrid[i][j].pixelCount / (float) (boxWidth*boxHeight);
			theGrid[i][j].isFull = ( theGrid[i][j].percentFull > percentFullThreshhold ) ? true : false;
		}
	}
}


int maxCountedSeedDepth = 0;
int maxMaxCountedSeedDepth = 0;

int poVisionGridAlgorithm::doBoxLabeling()
{
	// label all boxes with -1
	for( int i=0; i<gridWidth; i++ )
		for( int j=0; j<gridHeight; j++ )
			theGrid[i][j].label = -1;

	// set starting label 0
	int labelCount = 0;

	// reset maxMaxCountedSeedDepth 
	maxMaxCountedSeedDepth = 0;

	// search for seed box and do seedFillLabel
	for( int i=0; i<gridWidth; i++ )
	{
		for( int j=0; j<gridHeight; j++ )
		{
			if ( theGrid[i][j].isFull && theGrid[i][j].label == -1 )
			{
				// can't have more labels than shapes
				if ( labelCount < MAX_GRID_SHAPES-1 )
				{
					// reset maxCountedSeedDepth
					maxCountedSeedDepth = 0;

					// do seed fill
					seedFillLabel( i,j, labelCount, 0 );
					// increase label
					labelCount++;
					
					if ( maxCountedSeedDepth > maxMaxCountedSeedDepth )
						maxMaxCountedSeedDepth = maxCountedSeedDepth;
				}
			}
		}
	}

	//printf("max depth: %d\n", maxMaxCountedSeedDepth );

	return labelCount;
}


void poVisionGridAlgorithm::seedFillLabel( int i, int j, int label, int depth )
{
	// check max depth
	if ( depth > maxCountedSeedDepth )
		maxCountedSeedDepth = depth;

	// check in bounds
	if ( i<0 || i>=gridWidth || j < 0 || j >= gridHeight )
	{
		printf("seedFillLabel out of bounds (%d,%d)\n", i,j );
		return;
	}
	// label current box
	theGrid[i][j].label = label;

	// establish test matrix around point
	//int testMatrix[4][2] = { {-1,0}, {1,0}, {0,1}, {0,-1} };
	int testMatrix[8][2] = { {-1,0}, {1,0}, {0,1}, {0,-1}, {-1,-1}, {-1,1}, {1,-1}, {1,1} }; 

	// test surrounding points
	for( int n=0; n<8; n++ )
	{
		int i2 = i + testMatrix[n][0];
		int j2 = j + testMatrix[n][1];
		if ( i2 < 0 || i2 >= gridWidth || j2 < 0 || j2 >= gridHeight )
			continue;

		if ( theGrid[i2][j2].isFull && theGrid[i2][j2].label == -1 )
			seedFillLabel( i2, j2, label, depth+1 );
	}
}


void poVisionGridAlgorithm::buildShapeFromBoxes()
{
	// clear boxCount for all shapes
	for( int i=0; i<MAX_GRID_SHAPES; i++ )
		shapeList[i].boxCount = 0;

	// build shapes from labelled boxes
	for( int i=0; i<gridWidth; i++ )
	{
		for( int j=0; j<gridHeight; j++ )
		{
			int label = theGrid[i][j].label;
			if ( label == -1 )
			{
				theGrid[i][j].ownerShape = NULL;
				continue;
			}

			if ( label >= MAX_GRID_SHAPES )
			{
				//printf("ERROR: too many labels for max number of shapes\n"); 
				continue;
			}
			
			if ( shapeList[label].boxCount >= MAX_BOXES_PER_SHAPE )
			{
				//printf("ERROR: buildShapeFromBoxes: too many boxes per shape\n");
				continue;
			}

			// add box to shape's boxList
			int N = shapeList[label].boxCount;
			shapeList[label].boxList[N] = &theGrid[i][j];
			shapeList[label].boxCount++;
			// set boxes owner to shape
			theGrid[i][j].ownerShape = &shapeList[label];
		}
	}

	// print out number of boxes per shape every so often
	//if ( clock()%1000==0 )
	//{
	//	for( int i=0; i<shapeCount; i++ )
	//		printf("shape %d has %d boxes\n", i, shapeList[i].boxCount );
	//	if ( shapeCount > 0 )
	//		printf("\n");
	//}
}


void poVisionGridAlgorithm::findBestPointInShape( gridShape* S, poVisionImageBuffer* rawImage )
{

	//reset extremal points
	S->midPoint.set( 0,0,0 );
	S->leftPoint.set( 99999,0,0 );
	S->rightPoint.set( -99999,0,0 );
	S->bottomPoint.set( 0, 99999,0 );
	S->topPoint.set( 0, -99999,0 );
	
	float closestDist = 9999999;
	S->closestPoint.set(0,0,0);
	float furthestDist = 0;
	S->furthestPoint.set(0,0,0);
	poPoint V;

	S->brightestPoint.set( 0, -99999,0 );
	int brightestPixel = 0;
	
	int totalPixelCount = 0;
	
	// find all points
	for( int i=0; i<S->boxCount; i++ )
	{
		// find points
		gridBox* box = S->boxList[i];
		for( int j=0; j<box->pixelCount; j++ )
		{
			poVisPoint P = box->pixelList[j];
			
			// find brightest pixel
			if ( rawImage != NULL && P.x < rawImage->width && P.y < rawImage->height )
			{
				int pix = rawImage->getPixel(P.x,P.y);
				if ( pix > brightestPixel )
				{
					brightestPixel = pix;
					S->brightestPoint.set( P.x, P.y,0 );
				}
			}

			// accumulate for midPoint
			S->midPoint.x += P.x;
			S->midPoint.y += P.y;
			totalPixelCount++;
			
			// calculate extremal points
			if ( P.x < S->leftPoint.x )
				S->leftPoint.set( P.x, P.y,0 );
			
			if ( P.x > S->rightPoint.x )
				S->rightPoint.set( P.x, P.y,0 );
			
			if ( P.y < S->bottomPoint.y )
				S->bottomPoint.set( P.x, P.y,0 );
			
			if ( P.y > S->topPoint.y )
				S->topPoint.set( P.x, P.y,0 );
			
			// calculate closest and furthest
			V.set( P.x-referencePoint.x, P.y-referencePoint.y, 0 ); //use point
			//V.set( P.x-P.x, P.y-referencePoint.y,0 ); //use line
			float L = V.lengthSquared();
			if ( L < closestDist )
			{
				closestDist = L;
				S->closestPoint.set( P.x, P.y,0 );
			}
			if ( L > furthestDist )
			{
				furthestDist = L;
				S->furthestPoint.set( P.x, P.y,0 );
			}
		}
	}
	
	
	// calculate closest based upon furthest
	float furthestFromFurthestDistance = 0.0;
	S->closestPoint.set(0,0,0);
	for( int i=0; i<S->boxCount; i++ )
	{
		// find points
		gridBox* box = S->boxList[i];
		for( int j=0; j<box->pixelCount; j++ )
		{
			poVisPoint P = box->pixelList[j];
		
			V.set( P.x-S->furthestPoint.x, P.y-S->furthestPoint.y, 0 );
			float L = V.lengthSquared();
		
			if ( L > furthestFromFurthestDistance )
			{
				furthestFromFurthestDistance = L;
				S->closestPoint.set( P.x, P.y,0 );
			}
		}
	}
	
	
	// calculate midPoint
	if ( totalPixelCount > 0 )
	{
		S->midPoint.x *= 1.0 / (float ) totalPixelCount;
		S->midPoint.y *= 1.0 / (float ) totalPixelCount;
	}
	
	//calc major axis angle
	calculateMajorAxisAngle( S );

	/*
	//calculate center angle
	//angle from center to centroid
	float centerX = (S->rightPoint.x + S->leftPoint.x)/2;
	float centerY = (S->topPoint.y + S->bottomPoint.y)/2;
	float dX = S->midPoint.x-centerX;
	float dY = S->midPoint.y-centerY;
	float centAng = atan2( dY, dX ) * 180/3.14159;
	//printf("ang: %2f cx: %2f cy %2f\n", centAng, dX,dY );
	S->majorAxisAngle = centAng;
	*/
}


void	poVisionGridAlgorithm::calculateMajorAxisAngle( gridShape* S )
{
	//calc the angle to the point furthest from the midpoint, relative to the midpoint
	//poPoint furthestP = poPoint(0,0,0);
	//poPoint dFP =		poPoint(0,0,0);

	//fist we need to find the center of the group of pixels around the tip
	//calc the midpoint of the group of pixels only near the closestPoint
	int CDThresh = 10; //how big our group is
	poPoint cMid = poPoint(0,0,0);
	poPoint dD = poPoint(0,0,0);
	int totalPixelCount = 0;
	for( int i=0; i<S->boxCount; i++ )
	{
		// get pixels
		gridBox* box = S->boxList[i];

		for( int j=0; j<box->pixelCount; j++ )
		{
			poVisPoint P = box->pixelList[j];
			
			//only add the pixel if it is close enough to the 'closest point'
			float dDX = P.x-S->closestPoint.x;
			float dDY = P.y-S->closestPoint.y;
			dD.set( dDX,dDY,0 );
			if( dD.length() > CDThresh ) continue;

			/*
			//calc furthest point from mid
			float dFX = P.x-S->midPoint.x;
			float dFY = P.y-S->midPoint.y;
			dFP.set( dFX,dFY );
			if( dFP.length() > furthestP.length() )
			{
				furthestP.set( dFX,dFY ); //record
			}*/

			// accumulate for midPoint
			cMid.x += P.x;
			cMid.y += P.y;
			totalPixelCount++;
		}
	}
	// calculate cMid
	if ( totalPixelCount > 0 )
	{
		cMid.x *= 1.0 / (float ) totalPixelCount;
		cMid.y *= 1.0 / (float ) totalPixelCount;
	}

	


	//to find the 'angle' of the major and tip axis: 
	//calculate a vector for each pixel to the midpoint
	//add up the components into majorAxisTot
	//arctan majoraxistot for angle
	poPoint majorAxisTot;
	poPoint	tipAxisTot;
	float xyTot;
	float xyTotTip;
	float dMX;
	float dMY;
	float dTX;
	float dTY;
	for( int i=0; i<S->boxCount; i++ )
	{
		// get pixels
		gridBox* box = S->boxList[i];

		for( int j=0; j<box->pixelCount; j++ )
		{
			poVisPoint P = box->pixelList[j];

			//-----------------------for major axis
			// calculate closest and furthest
			dMX = P.x-S->midPoint.x;
			dMY = P.y-S->midPoint.y;

			xyTot += dMX*dMY;
			majorAxisTot.x += dMX*dMX;
			majorAxisTot.y += dMY*dMY;

			//-----------------------for tip axis
			// calculate closest and furthest
			dTX = P.x-cMid.x;
			dTY = P.y-cMid.y;
			
			//only add the pixel if it is close enough to the 'closet midpoint'
			dD.set( dTX,dTY,0 );
			if( dD.length() > CDThresh/2 ) continue;

			xyTotTip += dTX*dTY;
			tipAxisTot.x += dTX*dTX;
			tipAxisTot.y += dTY*dTY;
		}
	}
	//set major axis
	if( xyTot < 0 ) majorAxisTot.x *= -1;
	S->majorAxisAngle = atan2( majorAxisTot.y, majorAxisTot.x );

	//set tip axis
	//S->tipAxisAngle = atan2( furthestP.y, furthestP.x );

	if( xyTotTip < 0 ) tipAxisTot.x *= -1;
	S->tipAxisAngle = atan2( tipAxisTot.y, tipAxisTot.x );
}

void	poVisionGridAlgorithm::calculateContour( gridShape* S )
{
	int H = S->topPoint.x - S->bottomPoint.x;
	int W = S->rightPoint.x - S->leftPoint.x;
	float radius = ( H > W ) ? H*1.5 : W*1.5;
	
	// start with wide circular contour
	// shrink contour in using binary search
	for( int i=0; i<NUM_CONTOUR_POINTS; i++ )
	{
		// set up points for binary search
		poPoint A, B, C;
		A = S->midPoint;
		C.setPolar( i*360.0/NUM_CONTOUR_POINTS, radius );
		C += S->midPoint;
		B = (A+C)/2;
		
		// do binary search
		for( int n=0; n<7; n++ )
		{
			int indX = B.x / boxWidth;
			int indY = B.y / boxHeight;
			
			bool offGrid = false;
			if ( indX < 0 || indX >= gridWidth || indY < 0 || indY >= gridHeight )
				offGrid = true;

			if ( offGrid )							// if off grid, treat as not in shape (move right)
				C = B;
			else if ( theGrid[indX][indY].isFull && theGrid[indX][indY].ownerShape==S )	// if in shape and correctly owned, move left,
				A = B;
			else									// if not filled or owned by another shape, move right
				C = B;
			B = (A+C)/2;
		}
		
		S->contour[i] = C;
	}
	
	// smooth contour
	for ( int r=0; r<2; r++ )		// repeat twice
	{
		for( int i=0; i<NUM_CONTOUR_POINTS; i++ )
		{
			int prev = (i+NUM_CONTOUR_POINTS-1) % NUM_CONTOUR_POINTS;
			int next = (i+1) % NUM_CONTOUR_POINTS;
			S->contour[i] = S->contour[prev]*0.25 + S->contour[i]*0.5 + S->contour[next]*0.25;
		}
	}
}

void	poVisionGridAlgorithm::calculateDetailedContour( gridShape* S, poVisionImageBuffer* image )
{
	int H = S->topPoint.x - S->bottomPoint.x;
	int W = S->rightPoint.x - S->leftPoint.x;
	float radius = ( H > W ) ? H*1.5 : W*1.5;
	
	// start with wide circular contour
	// shrink contour in using binary search
	for( int i=0; i<NUM_CONTOUR_POINTS; i++ )
	{
		// set up points for binary search
		poPoint A, B, C;
		A = S->midPoint;
		C.setPolar( i*360.0/NUM_CONTOUR_POINTS, radius );
		C += S->midPoint;
		B = (A+C)/2;
		
		// do binary search
		for( int n=0; n<7; n++ )
		{
			int indX = B.x / boxWidth;
			int indY = B.y / boxHeight;
			
			bool offGrid = false;
			if ( indX < 0 || indX >= gridWidth || indY < 0 || indY >= gridHeight )
				offGrid = true;

			if ( offGrid )							// if off grid, treat as not in shape (move right)
				C = B;
			else if ( theGrid[indX][indY].isFull && theGrid[indX][indY].ownerShape==S )	// if in shape and correctly owned, move left,
				A = B;
			else									// if not filled or owned by another shape, move right
				C = B;

			B = (A+C)/2;

			//test if time to refine by looking at actual pixels
			int indXA = A.x / boxWidth;
			int indYA = A.y / boxHeight;
			int indXC = C.x / boxWidth;
			int indYC = C.y / boxHeight;
			float dX = A.x - C.x;
			float dY = A.y - C.y;
			//printf( "testing %d box sim: %d %d || %f %f || %f \n", i, indXA, indXC, A.x, C.x, dX);
			if( fabs(dX) < boxWidth/2 && fabs(dY) < boxHeight/2 ) //if we've converged on one box
			{	
				//after we find the outer edge, march in pix by pix
				//get the normalized dir
				poPoint CN = poPoint( C.x-S->midPoint.x,C.y-S->midPoint.y,0 );
				CN.normalize();
				CN *= -1; //reverse
			
				for( int p=0;p<boxWidth;p++ )
				{
					C += CN; //move in some
					int pIndX = C.x;
					int pIndY = C.y;
					
					//check bounds
					bool offPGrid = false;
					if ( pIndX < 0 || pIndX >= image->width || pIndY < 0 || pIndY >= image->height ) 
					{
						//printf( "Contour off pixel grid\n" );
						break;
					}
					else
					{
						//get pix
						unsigned char pixel = image->getPixel(pIndX,pIndY);

						if ( pixel > 128 )
						{
							//printf( "PIX MARCH: %d\n",p );
							break; //stop marching if its filled
						}
					}
				}
				//now we found the pixel coordinate, so break out of the binary search
				break;
			}
		}
		
		S->contour[i] = C;
	}
/*
	// smooth contour
	for ( int r=0; r<2; r++ )		// repeat twice
	{
		for( int i=0; i<NUM_CONTOUR_POINTS; i++ )
		{
			int prev = (i+NUM_CONTOUR_POINTS-1) % NUM_CONTOUR_POINTS;
			int next = (i+1) % NUM_CONTOUR_POINTS;
			S->contour[i] = S->contour[prev]*0.25 + S->contour[i]*0.5 + S->contour[next]*0.25;
		}
	}
*/	
}


