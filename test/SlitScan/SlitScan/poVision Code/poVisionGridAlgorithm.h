#pragma once

#include "poPoint.h"
#include "poColor.h"
//#include "poCommon.h"
#include "poVisionImageBuffer.h"
//#include "poCameraConnection.h"

class poVisPoint;
class gridBox;
class gridShape;

#define MAX_GRID_SHAPES	500

class poVisionGridAlgorithm
{
public:
	poVisionGridAlgorithm( int vWidth, int vHeight, int boxSize );
	~poVisionGridAlgorithm(void);

	void draw( int x, int y, float s );
	void process( poVisionImageBuffer* image, poVisionImageBuffer* rawImage );
	
	void doBoxFilling( poVisionImageBuffer* image );
	int  doBoxLabeling();
	void seedFillLabel( int i, int j, int label, int depth );
	void buildShapeFromBoxes();
	void findBestPointInShape( gridShape* S, poVisionImageBuffer* rawImage );
	void calculateContour( gridShape* S );
	void calculateDetailedContour( gridShape* S, poVisionImageBuffer* image );
	void calculateMajorAxisAngle( gridShape* S );
	
	float				percentFullThreshhold;

	int					videoWidth, videoHeight;
	int					boxWidth, boxHeight;
	int					gridWidth, gridHeight;

	gridBox**			theGrid;
	
	int					shapeCount;
	gridShape*			shapeList;

	poPoint				referencePoint;
	float				referenceLine;
};




class poVisPoint
{
public:
	int x, y;
	poVisPoint():x(0),y(0){}
	poVisPoint(int x, int y):x(x),y(y){}
	
	void set(int x, int y){this->x=x;this->y=y;}
};
int poVisPointDist(const poVisPoint& p1, const poVisPoint& p2);

class gridBox
{
public:
	gridBox();
	
	void		setup( int boxWidth, int boxHeight );
	
	int			x, y;
	int			indX,indY;
	int			pixelCount;
	poVisPoint*	pixelList;
	float		percentFull, percentFullLag;
	bool		isFull;
	int			label;
	
	gridShape*	ownerShape;
};

class gridShape
{
public:
	gridShape();
	
	gridBox*		removeBox( gridBox* B );
	
	int				boxCount;
	gridBox**		boxList;
	
	poPoint			midPoint;
	poPoint			leftPoint, rightPoint, topPoint, bottomPoint;
	poPoint			closestPoint, furthestPoint;
	poPoint			brightestPoint;
	float			majorAxisAngle;
	float			tipAxisAngle;
	
#define NUM_CONTOUR_POINTS 30
	poPoint			contour[NUM_CONTOUR_POINTS];
	
	bool			trackedForward;
	int				trackedShapeID;
	int				trackedAge;
	poPoint			immediateVelocity, averageVelocity;
    int             lastClickTime;
	poPoint			lastClickPoint;
    
	static int		trackedID_Generator;
};

