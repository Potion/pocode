
#include "poCameraCalibrator.h"
#include "poPoint.h"


// ============================ poCameraCalibrator ============================== //

poCameraCalibrator::poCameraCalibrator()
{
	isCalibrated = true;
}


// ============================ poTableCalibrator ============================== //

poTableCalibrator::poTableCalibrator() :
	finished(false),
	alignGridWidth(0),
	alignGridHeight(0),
	alignPointGrid(NULL),
	currentPoint(0),
	wait(1),
	percentDone(0.0),
	state(STARTING),
	now(0), then(0)
{
	alignPointGrid = NULL;
	strcpy( fileName, "" );
}

void poTableCalibrator::draw()
{
	if(alignPointGrid == NULL)
		return;
	
	updateClock();
	drawGrid();
}

void poTableCalibrator::drawCalibrated()
{
	glColor3f(0.8f, 0.85f, 0.0f);
	glPointSize(6);
	glBegin(GL_POINTS);
	for( int i=0; i<alignGridWidth*alignGridHeight; i++ )
	{		
		poPoint transformed = computeGridCoords(alignPointGrid[i].dist);
		glVertex2f(transformed.x, transformed.y);
	}
	glEnd();
}

poPoint poTableCalibrator::transformPoint( poPoint P )
{
	return computeGridCoords(P);
}

void	poTableCalibrator::takeSamplePoint( float x, float y )
{
	lastSamplePoint.set( x,y );
	//printf("sample point %f %f\n", x,y );
}


void poTableCalibrator::saveCalibration()
{
	FILE* dat = fopen(fileName, "w");
	if(dat == NULL) 
	{
		printf("can't save to calibration file %s\n", fileName );
		exit(-1);
	}

	printf("CALIBRATOR saving file %s\n", fileName );
	int nPoints = alignGridWidth*alignGridHeight;
	fprintf(dat, "%i %i\n", alignGridWidth, alignGridHeight);
	for(int i=0; i<nPoints; i++)
	{
		alignPointStruct point = alignPointGrid[i];
		fprintf(dat, "%f %f %f %f %f %f\n", point.orig.x, point.orig.y, point.orig.z, point.dist.x, point.dist.y, point.dist.z);
	}

	fclose(dat);
}

void poTableCalibrator::readCalibration()
{
	FILE* dat = fopen(fileName, "r");
	if(dat == NULL) 
	{
		printf("can't open calibration file %s\n", fileName );
		exit(-10);
	}
	printf("CALIBRATOR reading file %s\n", fileName );
	fscanf(dat, "%i %i", &alignGridWidth, &alignGridHeight);
	alignPointGrid = new alignPointStruct[alignGridWidth * alignGridHeight];
	for(int i=0; i<alignGridWidth*alignGridHeight; i++)
	{
		alignPointStruct point;
		fscanf(dat, "%f %f %f %f %f %f", &point.orig.x, &point.orig.y, &point.orig.z, &point.dist.x, &point.dist.y, &point.dist.z);
		//printf("%f %f %f %f %f %f\n", point.orig.x, point.orig.y, point.orig.z, point.dist.x, point.dist.y, point.dist.z);
	
		point.orig.x += poRand(-1.0,1.0); //add randomness to prevent lockup
		point.orig.y += poRand(-1.0,1.0);
		point.dist.x += poRand(-1.0,1.0);
		point.dist.y += poRand(-1.0,1.0);

		alignPointGrid[i] = point;
	}
	finalize();

	fclose(dat);
}

void poTableCalibrator::reset()
{
	/*if(currentPoint > 0)
		currentPoint = currentPoint - 1;
	alignPointGrid[currentPoint].recorded = false;
	state = STARTING;*/

	state = STARTING;
	finished = false;
	for( int i=0; i<alignGridWidth*alignGridHeight; i++ )
		alignPointGrid[currentPoint].recorded = false;
}

void poTableCalibrator::setUpAlignGridPoints( int L, int B, int R, int T, int numColumns, int numRows )
{
	alignGridWidth = numColumns;
	alignGridHeight = numRows;
	
	alignPointGrid = new alignPointStruct [ alignGridWidth*alignGridHeight ];

	int x = 0, y = 0;
	for( int i=0; i<alignGridWidth*alignGridHeight; i++ )
	{
		alignPointGrid[i].recorded = false;
		alignPointGrid[i].orig.set( L + x*(R-L)/(alignGridWidth-1),
									B + y*(T-B)/(alignGridHeight-1), 0 );
		alignPointGrid[i].dist.set( 0,0,0 );

		x++;
		if ( x==alignGridWidth )
		{
			y++;
			x = 0;
		}
	}
	
	finished = false;
	state = STARTING;
}

void poTableCalibrator::recordPoint()
{	
	printf("CALIBRATOR recording point %d:  %.1f %.1f\n", currentPoint, lastSamplePoint.x, lastSamplePoint.y );

	alignPointGrid[currentPoint].dist = lastSamplePoint;
	alignPointGrid[currentPoint].recorded = true;

	currentPoint += 1;
}

void poTableCalibrator::finalize()
{
	printf("CALIBRATOR finished calibration\n");
	finished = true;
	state = ALLDONE;
}

void poTableCalibrator::drawGrid()
{
	glPointSize(5);

	float aX, aY;
	// draw calibration grid
	for( int i=0; i<alignGridWidth*alignGridHeight; i++ )
	{
		if(state == ALLDONE)
			glColor3f(0.5f, 0.3f, 0.1f);
		else if(alignPointGrid[i].recorded)
			glColor3f(1.f, 1.f, 1.f);
		else
			glColor3f(0.f, 0.65f, 0.f);

		// drag orig points
		glBegin(GL_POINTS);
		aX = alignPointGrid[i].orig.x;
		aY = alignPointGrid[i].orig.y;
		glVertex2f(aX, aY);
		glEnd();

		// draw dist points
		glColor3f(0.1f, 0.3f, 0.5f);
		glBegin(GL_POINTS);
		aX = alignPointGrid[i].dist.x;
		aY = alignPointGrid[i].dist.y;
		glVertex2f(aX, aY);
		glEnd();
	}

	// draw arrows about set align point
	aX = alignPointGrid[currentPoint].orig.x;
	aY = alignPointGrid[currentPoint].orig.y;

	glColor3f(1.f, 1.f, 1.f);
	if(state == STARTING)
	{
		if(int(percentDone*10) % 2)
			glColor3f(1.f, 0.f, 0.f);
		else
			glColor3f(0.f, 1.f, 0.f);
	}
	
	if(!finished)
	{
		for( int r=0; r<4; r++ )
		{
			glPushMatrix();
			glTranslatef( aX,aY,0 );
			glRotatef( r*90,0,0,1 );
			glTranslatef( 5,0,0 );
			glBegin( GL_LINES );
			glVertex2f( 0,0 );
			float offset = 0;
			if(state == RECORDING)
				offset = 25 * percentDone;
			glVertex2f( 25-offset,0 );
			if(state != FINISHED)
			{
				glVertex2f( 0,0 );
				glVertex2f( 5,5 );
				glVertex2f( 0,0 );
				glVertex2f( 5,-5 );
			}
			glEnd();
			glPopMatrix();
		}
	}
}

void poTableCalibrator::drawCapturedPoints()
{
	glPointSize(5);
	
	float aX, aY;
	// draw calibration grid
	for( int i=0; i<alignGridWidth*alignGridHeight; i++ )
	{
		// draw dist points
		glColor3f( 0,1,0 );
		glBegin(GL_POINTS);
		aX = alignPointGrid[i].dist.x;
		aY = alignPointGrid[i].dist.y;
		glVertex2f(aX, aY);
		glEnd();
	}
}
								

void poTableCalibrator::updateClock()
{
	now = time(NULL);//glutGet(GLUT_ELAPSED_TIME);

	if(then == 0) 
		then = now;
	unsigned int elapsed = now - then;
	
	if(elapsed > wait)
		changeState();
	else
		percentDone = elapsed / (float)wait;
}

int longWait=3, shortWait=1;

void poTableCalibrator::changeState()
{
	percentDone = 0.0;
	then = time(NULL);//glutGet(GLUT_ELAPSED_TIME);
	
	if(state == STARTING)
	{
		wait = longWait;
		state = RECORDING;
	} 
	else if(state == RECORDING)
	{
		wait = shortWait;
		state = FINISHED;
	} 
	else if(state == FINISHED) 
	{
		recordPoint();
		if(currentPoint >= alignGridWidth*alignGridHeight)
		{
			finalize();
			saveCalibration();
		}
		else
			state = STARTING;
		wait = shortWait;
	}
	else if (state == ALLDONE)
	{
		return;
	}
}


poPoint	poTableCalibrator::computeGridCoords( poPoint &P )
{
	for( int x=0; x<alignGridWidth-1; x++ )
	{
		for( int y=0; y<alignGridHeight-1; y++ )
		{
			int nA = y*alignGridWidth + x;
			int nB = (y+1)*alignGridWidth + x;
			int nC = y*alignGridWidth + (x+1);
			int nD = (y+1)*alignGridWidth + (x+1);
			
			poPoint R = computeDistortCoords( alignPointGrid[nA].dist, alignPointGrid[nB].dist,
											alignPointGrid[nC].dist, alignPointGrid[nD].dist, P );

			bool noLeftTest = false;
			bool noRightTest = false;
			bool noTopTest = false;
			bool noBottomTest = false;

			if ( x==0 )
				noLeftTest = true;
			if ( x==alignGridWidth-2 )
				noRightTest = true;
			if ( y==0 )
				noBottomTest = true;
			if ( y==alignGridHeight-2 )
				noTopTest = true;

			if ( (R.x >= 0 || noLeftTest) && (R.x <=1.0 || noRightTest) &&
				 (R.y >= 0 || noBottomTest) && (R.y <=1.0 || noTopTest) )
			{
				poPoint Av = alignPointGrid[nA].orig;
				poPoint Bv = alignPointGrid[nB].orig;
				poPoint Cv = alignPointGrid[nC].orig;
				poPoint Dv = alignPointGrid[nD].orig;

				poPoint S = Av*(1.0-R.x-R.y+R.x*R.y) + Bv*(R.y-R.x*R.y) + Cv*(R.x-R.x*R.y) + Dv*(R.x*R.y);
				return S;
			}
		}
	}

	poPoint Q;
	Q.set( 0,0,0 );
	return Q;
}

poPoint	poTableCalibrator::computeDistortCoords( poPoint &A, poPoint &B, poPoint &C, poPoint &D, poPoint &P )
{
	float a = P.x - A.x;
	float b = -A.x + C.x;
	float c = -A.x + B.x;
	float d = A.x - B.x - C.x + D.x;
	
	float e = P.y - A.y;
	float f = -A.y + C.y;
	float g = -A.y + B.y;
	float h = A.y - B.y - C.y + D.y;
	
	float Qa = -c*h + d*g;
	float Qb = a*h - c*f - d*e + g*b;
	float Qc = a*f - b*e;
	
	float disc = Qb*Qb - 4*Qa*Qc;
	if ( disc < 0 )
	{
		//printf("negative discrimant: no solution\n");
		return poPoint(0,0,0);
	}
	
	float J1 = ( -Qb + sqrt(disc) ) / (2.0*Qa);
	float I1a = (a - c*J1) / (b + d*J1);
	
	float i = I1a;
	float j = J1;
        
        
    poPoint Res;
    Res.set( i,j,0 );
    return Res;
}

