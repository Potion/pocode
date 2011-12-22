

#include "poVisionCalibrator.h"


bool		triangleCalibrator::processPoint( poPoint P, poPoint& Result )
{
	float X0 = srcA.x;
	float Y0 = srcA.y;

	float X1 = srcB.x;
	float Y1 = srcB.y;
	
	float X2 = srcC.x;
	float Y2 = srcC.y;

	float topC = ( P.y - Y0 ) / ( Y1 - Y0 ) - ( P.x - X0 ) / ( X1 - X0 );
	float botC = ( X0 - X2 ) / ( X1 - X0 ) - ( Y0 - Y2 ) / ( Y1 - Y0 );
	float C = topC / botC;

	float B = ( P.x - X0 + C*(X0-X2) ) / ( X1 - X0 );

	float A  = 1.0 - B - C;
	
	
	Result.x = destA.x*A + destB.x*B + destC.x*C;
	Result.y = destA.y*A + destB.y*B + destC.y*C;
	
	if ( A < 0.0 || B < 0.0 || C < 0.0 )
		return false;
	else
		return true;
}

void		triangleCalibrator::checkValidity()
{
}

void		triangleCalibrator::drawSrcTriangle( float tX, float tY, float scale  )
{
	glLineWidth(3);
	glPushMatrix();
	glTranslatef( tX, tY, 0 );
	glScalef( scale, scale, 1);

	glBegin( GL_LINE_LOOP );
	glColor3f( 1,0,0 );
	glVertex2f( srcA.x, srcA.y );
	glColor3f( 1,1,0 );
	glVertex2f( srcB.x, srcB.y );
	glColor3f( 0,1,1 );
	glVertex2f( srcC.x, srcC.y );
	glEnd();

	glPopMatrix();
}

void		triangleCalibrator::drawDestTriangle( float tX, float tY, float scale  )
{
	glLineWidth(5);
	glPushMatrix();
	glTranslatef( tX, tY, 0 );
	glScalef( scale, scale, 1);

	glColor3f( 0,1,0 );
	glBegin( GL_LINE_LOOP );
	glColor3f( 1,0,0 );
	glVertex2f( destA.x, destA.y );
	glColor3f( 1,1,0 );
	glVertex2f( destB.x, destB.y );
	glColor3f( 0,1,0 );
	glVertex2f( destC.x, destC.y );
	glEnd();

	glPopMatrix();
}


quadCalibrator::quadCalibrator()
{
	float W = 400;
	float H = 300;

	srcA.set( 50,50,0 );
	srcB.set( 50+W, 50,0 );
	srcC.set( 50+W, 50+H,0 );
	srcD.set( 50, 50+ H,0 );

	destA.set( 50,50,0 );
	destB.set( 50+W, 50,0 );
	destC.set( 50+W, 50+H,0 );
	destD.set( 50, 50+ H,0 );

	srcB += poPoint( poRand(0,5), poRand(),0 );
	srcD += poPoint( poRand(0,5), poRand(),0 );

	destB += poPoint( poRand(0,5), poRand() );
	destD += poPoint( poRand(0,5), poRand() );

	poPoint P;
	poPoint R;
	processPoint( P, R );
}

bool		quadCalibrator::processPoint( poPoint P, poPoint& Result )
{
	triA.srcA = srcA;
	triA.destA = destA;
	triA.srcB = srcB;
	triA.destB = destB;
	triA.srcC = srcC;
	triA.destC = destC;

	triB.srcA = srcA;
	triB.destA = destA;
	triB.srcB = srcD;
	triB.destB = destD;
	triB.srcC = srcC;
	triB.destC = destC;

	if ( triA.processPoint( P, Result ) )
		return true;
	if ( triB.processPoint( P, Result ) )
		return true;
	return false;
}

void		quadCalibrator::drawSrc(float tX, float tY, float scale  )
{
	triA.drawSrcTriangle( tX, tY, scale );
	triB.drawSrcTriangle( tX, tY, scale );
}


void		quadCalibrator::drawDest(float tX, float tY, float scale  )
{
	triA.drawDestTriangle( tX, tY, scale );
	triB.drawDestTriangle( tX, tY, scale );
}

float		quadCalibrator::distanceToPoint( poPoint P )
{
	poPoint	points[4];
	points[0] = srcA;
	points[1] = srcB;
	points[2] = srcC;
	points[3] = srcD;
	
	float minDist = 999999;
	for( int i=0; i<4; i++ )
	{
		poPoint dV = points[i] - P;
		float dist = dV.length();
		if ( dist < minDist	)
			minDist = dist;
	}
	
	return minDist;
}


meshCalibrator::meshCalibrator( int meshW, int meshH )
{
	meshWidth = meshW;
	meshHeight = meshH;

	srcMesh = new poPoint* [ meshWidth+1 ];
	for( int i=0; i<meshWidth+1; i++ )
		srcMesh[i] = new poPoint [ meshHeight+1 ];

	destMesh = new poPoint* [ meshWidth+1 ];
	for( int i=0; i<meshWidth+1; i++ )
		destMesh[i] = new poPoint [ meshHeight+1 ];

	quadMesh = new quadCalibrator* [ meshWidth ];
	for( int i=0; i<meshWidth; i++ )
		quadMesh[i] = new quadCalibrator [ meshHeight ];

	calibrationState = -1;
	calibrationFrameCount = 0;
} 


bool	meshCalibrator::processPoint( poPoint P, poPoint& Result )
{
	lastInputPoint = P;

	// test all quad meshes to see if point is inside
	// and if so process it
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			if( quadMesh[i][j].processPoint( P, Result ) )
				return true;
		}
	}
	
	// point not inside any quad, so find closest quad
	float minDist = 999999;
	quadCalibrator* bestQuad = NULL;
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			float dist = quadMesh[i][j].distanceToPoint(P);
			if ( dist < minDist )
			{
				minDist = dist;
				bestQuad = &quadMesh[i][j];
			}
		}
	}
	
	if ( bestQuad != NULL )
	{
		bestQuad->processPoint( P, Result );
		return true;
	}
	
	
	return false;
}


void	meshCalibrator::drawCalibration()
{
	//if ( calibrationState >= 0 )
	//	return;

	int count = 0;
	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			// draw destination point
			poPoint P = destMesh[i][j];
			if ( count == calibrationState )
			{
				if ( calibrationFrameCount > 40 )		
				{
					srcMesh[i][j] = lastInputPoint;		// capture the point
					glColor3f( 1,1,0 );
				}
				else
				{
					glColor3f( 0,1,0 );
				}
				glRectf( P.x-20, P.y-20, P.x+20, P.y+20 );				
			}
			else
			{
				glColor3f( 1,1,1 );
				glRectf( P.x-10, P.y-10, P.x+10, P.y+10 );
			}

			P = srcMesh[i][j];
			glColor3f( 0,1,1 );
			glRectf( P.x-3, P.y-3, P.x+3, P.y+3 );
			
			count++;
		}
	}

	calibrationFrameCount++;
	if ( calibrationFrameCount == 50 && calibrationState >= 0 )
	{
		// go to next state
		calibrationState++;
		calibrationFrameCount = 0;
		// at end
		if ( calibrationState >= (meshWidth+1)*(meshHeight+1) )
		{
			setupSrcMesh();		// build quad mesh from captured points
			write();
			calibrationState = -1;
			return;
		}
	}
}

void	meshCalibrator::calibrationClick( int mX, int mY )
{
	printf("calibrationClick %d %d\n", mX, mY );
	if ( calibrationState < 0 )
		return;

	int count = 0;
	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			if ( count == calibrationState )
				srcMesh[i][j] = poPoint( mX, mY );
			count++;
		}
	}
	
	calibrationState++;
	printf("calibrationState %d\n", calibrationState );
	if ( calibrationState >= (meshWidth+1)*(meshHeight+1) )
	{
		setupSrcMesh();		// build quad mesh from captured points
		write();
		calibrationState = -1;
		return;
	}

}

void	meshCalibrator::setupDestMesh( float X0, float Y0, float X1, float Y1 )
{
	// setup mesh points
	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			float dX = (X1-X0) / (float) (meshWidth) * (float) i;
			float dY = (Y1-Y0) / (float) (meshHeight) * (float) j;
			destMesh[i][j].set( X0+dX, Y0+dY, 0 );
		}
	}

	// setup quad meshes based upon mesh points
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			quadMesh[i][j].destA = destMesh[i][j];
			quadMesh[i][j].destB = destMesh[i+1][j];
			quadMesh[i][j].destC = destMesh[i+1][j+1];
			quadMesh[i][j].destD = destMesh[i][j+1];
			poPoint P;
			poPoint R;
			quadMesh[i][j].processPoint( P, R );
		}
	}
}

void	meshCalibrator::setupSrcMesh()
{
	// setup quad meshes based upon mesh points
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			quadMesh[i][j].srcA = srcMesh[i][j];
			quadMesh[i][j].srcB = srcMesh[i+1][j];
			quadMesh[i][j].srcC = srcMesh[i+1][j+1];
			quadMesh[i][j].srcD = srcMesh[i][j+1];
			poPoint P;
			poPoint R;
			quadMesh[i][j].processPoint( P, R );
		}
	}
}


void	meshCalibrator::startCalibration()
{
	calibrationState = 0;
	calibrationFrameCount = 0;

	// clear source points
	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			srcMesh[i][j] = poPoint(0,0,0);
		}
	}
	
	// reset mesh
	setupSrcMesh();
}


void	meshCalibrator::acceptCalibrationPoint( poPoint )
{
}


void	meshCalibrator::drawSrc( float tX, float tY, float scale )
{
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			quadMesh[i][j].drawSrc( tX, tY, scale );
		}
	}
}


void	meshCalibrator::drawDest( float tX, float tY, float scale )
{
	for( int i=0; i<meshWidth; i++ )
	{
		for( int j=0; j<meshHeight; j++ )
		{
			quadMesh[i][j].drawDest( tX, tY, scale );
		}
	}
}


void	meshCalibrator::write()
{
	FILE* F = fopen("local/mesh_calib.txt", "w" );

	if ( F==NULL )
	{
		printf("ERROR: could open mesh_calib.txt to write\n");
		return;
	}

	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			fprintf( F, "%f %f\n", srcMesh[i][j].x, srcMesh[i][j].y );
		}
	}
	fclose(F);

	printf("wrote mesh_calib.txt file\n");
}


void				meshCalibrator::read()
{
	FILE* F = fopen("local/mesh_calib.txt", "r" );

	if ( F==NULL )
	{
		printf("ERROR: could open mesh_calib.txt to read\n");
		return;
	}

	for( int i=0; i<meshWidth+1; i++ )
	{
		for( int j=0; j<meshHeight+1; j++ )
		{
			fscanf( F, "%f %f\n", &srcMesh[i][j].x, &srcMesh[i][j].y );
			srcMesh[i][j] += poPoint(poRand(-1,1), poRand(-1,1) );
		}
	}
	fclose(F);

	setupSrcMesh();
}

