/*
 *  poVisionSystem.h
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poPoint.h"
#include "poVisionImageBuffer.h"
#include "poVisionImageProcs.h"
#include "poVisionGridAlgorithm.h"
//#include "poCameraConnection.h"
#include "poTextBox.h"
#include "poShape2D.h"
//#include "poShapePrimitives.h"

// #include "FreeImage.h"

enum visionMethodType { BACKGROUND_SUBTRACTION, EDGE_DETECTION }; 


class poVisionSystem
{
public:
	poVisionSystem( int vWidth, int vHeight, int camID );

	//Functions
	virtual void			update(){};
	virtual void			draw(){};//( int x, int y ){};
	virtual void			drawVariables(int Mx, int My){};//(int x, int y){};

	virtual void			editMask(float x, float y, bool click, bool dragging){};
	virtual void			addMaskPoint(float x, float y){};
	virtual void			drawOnMask(float x, float y, bool dragging){};
	virtual void			deleteRecentMaskPoint(){};
	virtual void			clearMaskPoints(){};
	virtual void			saveMask(){};
	
	void					process( unsigned char* buffer );
	//void					sendDataToCameraConnection( int cameraID );
	//void					sendShapesToCameraConnection( int cameraID );
	virtual void			resetBuffers();	
	virtual void			readConfigFile();
	virtual void			WriteConfigFile();
	virtual void			handleEditKey(int key){};

	//Functions to give direct access to the grid
	bool					getBox(int i, int j);
	int						get_videoWidth();
	int						get_videoHeight();	
	int						get_boxWidth();
	int						get_boxHeight();
	int						get_gridWidth();
	int						get_gridHeight();
	void					setDrawCoordinates(float x, float y){DrawX = x; DrawY = y;};

	int						camID;

	int						threshold;
	poPoint					refPoint;
	int						gridBoxSize;
	int						objectMinBoxSize;
	float					accumUpdateSpeed;
	int						accumRejectionRange;

	poVisionImageBuffer*	rawBuffer;
	poVisionImageBuffer*	maskBuffer;
	
	int						thresholdMIN;
	int						thresholdMAX;
	bool					usingMAP;

	float					DrawX, DrawY;
	int						DrawMode; //0 is shape, 1 is pencil
	int						DrawColor; //0 or 255
	int						DrawWidth;
	
	static bool overrideCheckSum;
	
protected:
	poVisionGridAlgorithm*		gridAlgorithm;
	
	int							previousChecksum;
	
	visionMethodType			visionMethod;
	poPoint*					Ref[2][2];
};


class poVisionSubstractionSystem : public poVisionSystem
{
public:
	poVisionSubstractionSystem( int vWidth, int vHeight, int camID );

	virtual void			update();
	virtual void			draw();// int x, int y );
	virtual void			drawVariables(int Mx, int My);//int x, int y);
	virtual void			resetBuffers();
	
	virtual void			addMaskPoint(float x, float y);
	virtual void			drawOnMask(float x, float y, bool dragging);
	virtual void			editMask(float x, float y, bool click, bool dragging);
	virtual void			deleteRecentMaskPoint();
	virtual void			clearMaskPoints();
	virtual void			saveMask();
		
	void					SetToUseThreshMap(bool yes);
	void					SetThreshold(int newval);
	void					SetMIN(int newMIN);	
	void					SetMAX(int newMAX);
	void					SetMINMAX(int newMIN, int newMAX);
	void					SetAccumSpeed(float newval);
	void					SetAccumReject(int newval);
	virtual void			handleEditKey(int key);
	
	void updateText(poTextBox* TXT, std::string* label, float fval);
	void updateText(poTextBox* TXT, std::string* label, int ival);
	void updateText(poTextBox* TXT, std::string* label, std::string sval);
	
	void GenerateMask();


	poVisionAndProc*				maskProc;
	poVisionBlurProc*				blurProc;
	poVisionAccumulateProc*			accumProc;
	poVisionDifererenceProc*		diffProc;
	//Threshold Procs
	poVisionImageProc* threshMETHOD;
	//bool usingMAP;
	poVisionThresholdProc*			threshProc;
	poVisionThresholdMAPProc*		threshMapProc;
	
	//ThresholdMap Variables
	poVisionImageBuffer*			threshMapBuffer;
	poVisionImageBuffer*			threshMapBufferACTIVE;
	
	//debugging text
	poTextBox*						label[4];
	poTextBox*						variables[6];
	std::string*					MAPvariableLabels[6];
	std::string*					NONMAPvariableLabels[6];
	poTextBox*						FixedVariables;
	poTextBox*						Instructions;
	int								EditMode;

	poShape2D*						maskPTS;
	// poRectShape*					OuterMask;
};


class poVisionEdgeDetectionSystem : public poVisionSystem
{
public:
	poVisionEdgeDetectionSystem( int vWidth, int vHeight, int camID );

	virtual void			update();
	virtual void			draw( int x, int y );

	virtual void			resetBuffers();

	poVisionAndProc*			maskProc;
	poVisionBlurProc*			blurProc;
	poVisionAccumulateProc*		accumProc;

	poVisionEdgeDetectProc*		edgeDetectProc1;
	poVisionThresholdProc*		threshProc1;
	poVisionDilateProc*			dilateProc;
	poVisionInvertProc*			invertProc;
	
	poVisionEdgeDetectProc*		edgeDetectProc2;
	poVisionThresholdProc*			threshProc2;
	
	poVisionAndProc*						andProc;
	poVisionErodeProc*					erodeProc;

};
