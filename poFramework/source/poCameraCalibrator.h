

#ifndef PO_CAMERA_CALIBRATOR
#define PO_CAMERA_CALIBRATOR

#include "poCommon.h"
#include "poObject.h"


class poCameraCalibrator : public poObject
{
public:
	poCameraCalibrator();

	virtual void draw() { };
	virtual void drawCapturedPoints() { };
	
	virtual poPoint transformPoint( poPoint P ) { return P; };		// this can and should be subclassed
	virtual void	takeSamplePoint( float x, float y ) { };
	virtual bool	isFinished() { return true; };
	virtual void	reset() { };

	virtual void	saveCalibration() { };
	virtual void	readCalibration() { };

	bool isCalibrated;
};

struct alignPointStruct
{
	bool recorded;
	poPoint orig;
	poPoint dist;
};

enum CalibratorState {STARTING, RECORDING, FINISHED, ALLDONE};

class poTableCalibrator : public poCameraCalibrator
{
public:
	poTableCalibrator();

	virtual void draw();
	virtual void drawCapturedPoints();
	
	virtual poPoint transformPoint( poPoint P );
	virtual void	takeSamplePoint( float x, float y );
	virtual bool	isFinished() { return finished; };
	virtual void	reset();

	void	setUpAlignGridPoints( int L, int B, int R, int T, int numColumns, int numRows );

	void	setFileName( char* S ) { strcpy(fileName,S); };
	virtual void	saveCalibration();
	virtual void	readCalibration();

private:
	void	drawGrid();
	void	drawCalibrated();
	void	recordPoint();
	void	finalize();

	poPoint computeGridCoords(poPoint &P);
	poPoint computeDistortCoords( poPoint &A, poPoint &B, poPoint &C, poPoint &D, poPoint &P );
	
	void	updateClock();
	void	changeState();

	poPoint				lastSamplePoint;
	int					alignGridWidth, alignGridHeight;
	alignPointStruct	*alignPointGrid;

	int					currentPoint;
	CalibratorState		state;
	bool				finished;
	
	double				percentDone;
	unsigned			int now, then, wait;
	char				fileName[512];
};


#endif