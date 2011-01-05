/*
 *  poLayout.h
 *  poFramework_static_lib
 *
 *  Created by Young Cho on 7/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poObjectModifier.h"
#include "poObject.h"
#include "poShape2D.h"
#include "poTextBox.h"


class poTextOnPath : public poTextBox
{
public:
	poTextOnPath();
	poTextOnPath(int width, int height);
	poTextOnPath(const char* text, int width, int height);
	poTextOnPath(const char* text, int width, int height, const poColor& fontColor );
	poTextOnPath(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor );
	virtual	~poTextOnPath();
	
	virtual void		draw();
	
	void				construct();
	void				addPoint( float x, float y );
	void				addShape( poShape2D* shape );
	void				calculatePath();
	void				updateLayout();
	
	
	std::vector<poPoint>		pointList;
	poTBLetterVector			pathLetterList;
	
	poPoint						offset;
	float						pathLength;
	bool						loopText;
};


class poStampPath : public poObjectModifier
{
public:
	poStampPath();
	
	virtual void			setup(poObject*);
	virtual void			setdown(poObject*);
	
	void					addShape( poObject* obj );
	
	poObject*				stampObj;
};




class poAnimationPath : public poObjectModifier
{
public:
	poAnimationPath();
	
	virtual void			setup(poObject*);
	virtual void			setdown(poObject*);
	
	void					updatePoint(poObject* obj);
	void					updatePath();
	void					drawPath();
	
	void					addPoint(float x, float y);
	void					addShape( poShape2D* shape );

	float					motionPercent;
	
	bool					doDrawing;
	poColor					strokeColor;
	int						strokeWidth;
	
	bool					loop;
	float					totalPathLength;
	
	std::vector<poPoint>	pointList;
	poPoint					objPosition;
};

enum poAnimationStyle { 
	PO_JUMP=0,
	PO_LINEAR,
	PO_EASE_IN, 
	PO_EASE_OUT, 
	PO_EASE_IN_OUT, 
	PO_JITTER, 
	PO_OVERSHOOT, 
	PO_LAZY_EASE_IN_OUT 
};

class poTimeLine : public poObject
{
public:
	poTimeLine();
	
	virtual void	update();
	
	void			captureFrame();
	void			setFrame( int frame, poAnimationStyle animationType, float emphasis, int duration );
	void			setFrame( float frame, poAnimationStyle animationType, float emphasis, int duration );
	int				getNumFrames(){ return childrenPropertyList.size(); };
	
	poObjectPropertyVector					thisPropertyList;
	poObjectProperties						thisStart;
	poObjectProperties						thisEnd;
	
	std::vector<poObjectPropertyVector>		childrenPropertyList;
	poObjectPropertyVector					childrenStartList;
	poObjectPropertyVector					childrenEndList;
	poAnimationStyle						animationType;
	
	int						timer;
	int						startTime;
	int						currFrame;
	int						duration;
	
	float					emphasis;
	float					motionPercent;
};



class poLayout : public poObjectModifier
{
public:
	poLayout();
	
	~poLayout(){};
	
	virtual void setup(poObject*) { };
	virtual void setdown(poObject*) { };
	
	virtual void init();
	
	virtual void setAlign( poHoriOrientation hAlign, poVertOrientation vAlign);
	virtual void doLayout(poObject*) { };
	
	bool		layoutFinished;
	
	poHoriOrientation hAlign;
	poVertOrientation vAlign;
};


class poGridLayout : public poLayout
{
public:
	poGridLayout(){};
	poGridLayout( int numCols, int numRows, float spacing );
	poGridLayout( int numCols, int numRows, float horiSpacing, float vertSpacing );
	
	void			construct( int numCols, int numRows, float horiSpacing, float vertSpaing );
	
	virtual void	setup(poObject*);
	virtual void	setdown(poObject*);
	
	virtual void	doLayout( poObject* obj );
	virtual void	drawLayout();
	
	int				numRows, numCols;
	float			horiSpacing, vertSpacing;
	float			gridW, gridH;
	float*			rowMaxHeight;
	float*			colMaxWidth;
	
	bool			doDrawing;
	poColor			strokeColor;
	int				strokeWidth;
};

class poHorizontalLayout : public poGridLayout
{
public:
	poHorizontalLayout();
	poHorizontalLayout(float spacing);
	
	virtual void setup(poObject*);
	virtual void setdown(poObject*);
};

class poVerticalLayout : public poGridLayout
{
public:
	poVerticalLayout();
	poVerticalLayout(float spacing);
	
	virtual void setup(poObject*);
	virtual void setdown(poObject*);
};