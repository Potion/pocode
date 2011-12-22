/*
 *  poVisionImageProcs.h
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poVisionImageBuffer.h"

class poVisionImageProc
{
public:
	poVisionImageProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer ) { };
	virtual void			process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 ) { };
	
	void					processProcOutput( poVisionImageProc* inputProc )  { process(inputProc->outputBuffer); };
	void					processProcOutput( poVisionImageProc* inputProc1, poVisionImageProc* inputProc2 ) { process(inputProc1->outputBuffer,inputProc2->outputBuffer); };
		
	poVisionImageBuffer*	outputBuffer;
};


class poVisionBackSubProc : public poVisionImageProc
{
public:
	poVisionBackSubProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
	
	poVisionImageBuffer*	backBuffer;
	int						framesUntilGrab;
};


class poVisionEdgeDetectProc : public poVisionImageProc
{
public:
	poVisionEdgeDetectProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
	
	int						edgeReach;
};

class poVisionAccumulateProc : public poVisionImageProc
{
public:
	poVisionAccumulateProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
	
	int						doFullCapture;
	float					updateSpeed;
	
	bool					doAssymmetricUpdate;
	float					lighterUpdateSpeed;
	float					darkerUpdateSpeed;

	float*					floatBuffer;

	int						rejectionRange;
};

class poVisionDifererenceProc : public poVisionImageProc
{
public:
	poVisionDifererenceProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 );

	int						bias;		// -1 = only detect darker, 0 = detect all, 1 detect only lighter
};


class poVisionThresholdProc : public poVisionImageProc
{
public:
	poVisionThresholdProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
	
	int		threshold;
};

class poVisionThresholdMAPProc : public poVisionImageProc
	{
	public:
		poVisionThresholdMAPProc( int width, int height );
		
		virtual void process(  poVisionImageBuffer* inputBuffer);
		void setMINMAX( int MIN_, int MAX_);
		void setMap();
		
		poVisionImageBuffer*	MapBufferACTIVE;
		poVisionImageBuffer*	MapBufferREFERENCE;
		int	thresholdMAX, thresholdMIN;
		float thresholdDIFF;
	};

class poVisionErodeProc : public poVisionImageProc
{public:
	poVisionErodeProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
};


class poVisionDilateProc : public poVisionImageProc
{
public:
	poVisionDilateProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
};


class poVisionBlurProc : public poVisionImageProc
{
public:
	poVisionBlurProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );

	int						blurRepeat;
	poVisionImageBuffer*	tmpBuffer;
};


class poVisionInvertProc : public poVisionImageProc
{
public:
	poVisionInvertProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );
};


class poVisionAndProc : public poVisionImageProc
{
public:
	poVisionAndProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 );
};


class poVisionOrProc : public poVisionImageProc
{
public:
	poVisionOrProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* inputBuffer2 );
};


class poVisionConvolutionProc : public poVisionImageProc
{
public:
	poVisionConvolutionProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer1, poVisionImageBuffer* convBuffer );
	
	bool					doSumOfDifferences;
};


class poVisionMOSnoiseProc : public poVisionImageProc
{
public:
	poVisionMOSnoiseProc( int width, int height );
	
	virtual void			process( poVisionImageBuffer* inputBuffer );

};


