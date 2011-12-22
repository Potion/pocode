#pragma once


#include "poPoint.h"
#include "poObject.h"


class	triangleCalibrator
{
public:
	poPoint		srcA, srcB, srcC;
	poPoint		destA, destB, destC;

	bool		processPoint( poPoint P, poPoint& Result );
	void		checkValidity();

	void		drawSrcTriangle( float tX, float tY, float scale  );
	void		drawDestTriangle( float tX, float tY, float scale  );
};


class	quadCalibrator
{
public:
	quadCalibrator();

	bool		processPoint( poPoint P, poPoint& Result );

	void		drawSrc( float tX, float tY, float scale );
	void		drawDest( float tX, float tY, float scale  );

	float		distanceToPoint( poPoint P );
	
	triangleCalibrator	triA;
	triangleCalibrator	triB;
	
	poPoint		srcA, srcB, srcC, srcD;
	poPoint		destA, destB, destC, destD;
};

class	meshCalibrator
{
public:
	meshCalibrator( int meshW, int meshH );

	bool				processPoint( poPoint P, poPoint& Result );

	void				setupDestMesh( float X0, float Y0, float X1, float Y1 );
	void				setupSrcMesh();		// assumes point srcMesh points exist

	void				startCalibration();
	void				acceptCalibrationPoint( poPoint );
	void				drawCalibration();
	void				calibrationClick( int mX, int mY );

	void				drawSrc( float tX, float tY, float scale );
	void				drawDest( float tX, float tY, float scale );

	void				write();
	void				read();

	int					meshWidth, meshHeight;

	poPoint**			srcMesh;
	poPoint**			destMesh;
	quadCalibrator**	quadMesh;

	int					calibrationState;
	int					calibrationFrameCount;

	poPoint				lastInputPoint;
};