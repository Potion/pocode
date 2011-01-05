/*
 *  poShapePrimitives.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/13/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */


#ifndef _PO_SHAPE_PRIMITIVES_H_
#define _PO_SHAPE_PRIMITIVES_H_

#include "poShape2D.h"

class poRectShape : public poShape2D
{
public:
	poRectShape();
	poRectShape(int width, int height);
	poRectShape(int width, int height, const poColor& fillColor);
	poRectShape(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
	
	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poRectShape"; };
	virtual void		init();
	
	void	reshape( float W, float H );
	float	getWidth()  { return rectWidth; };
	float	getHeight()  { return rectHeight; };

protected:
	float	rectWidth, rectHeight;
	void	construct(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
};


class poOvalShape : public poShape2D
{
public:
	poOvalShape();
	poOvalShape(int radius);
	poOvalShape(int radius, const poColor& fillColor);
	poOvalShape(int radius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
	poOvalShape(int width, int height);
	poOvalShape(int width, int height, const poColor& fillColor);
	poOvalShape(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
	
	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poOvalShape"; };
	virtual void		init();

	void	reshape( float W, float H );
	
//private:
	float ovalWidth, ovalHeight;
	void construct(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
};



class poRoundRectShape : public poShape2D
{
public:
	poRoundRectShape();
	poRoundRectShape(int width, int height, float cornerRadius);
	poRoundRectShape(int width, int height, float cornerRadius, const poColor& fillColor);
	poRoundRectShape(int width, int height, float cornerRadius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);

	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poRoundRectShape"; };
	virtual void		init();
	
	void	reshape( float W, float H, float rad );
	void	reshape( float W, float H );
	
private:
	float rectWidth, rectHeight, cornerRadius;
	void construct(int width, int height, float cornerRadius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
};



class poLineShape : public poShape2D
{
public:
	poLineShape();
	poLineShape( poPoint A, poPoint B );

	
	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poLineShape"; };
	virtual void		init();
	
	void	reshape( poPoint A, poPoint B );

private:
	poPoint	A, B;
	void	construct( poPoint A, poPoint B );
};

class poBezierShape : public poShape2D
{
public:
	poBezierShape();
	
	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poLineShape"; };
	virtual void		init();
	
	virtual void		update();
	virtual void		addPoint(float x, float y);
	
	void				generateBezierPoints();
	
	bool				bezierGenerated;
	int					numPointsPerLine;
	poPointVector		bezierPointList;

private:
	void	construct();	
};

class poAnimatedStrokeShape : public poShape2D
{
public:
	poAnimatedStrokeShape();
	
	virtual void		draw();
	
	void				addShape( poShape2D* shape );
	
	float				strokePercent;
	float				totalLength;
private:
	void	construct();	
};

class poImageShape : public poRectShape
{
public:
	poImageShape();
	poImageShape( char* imageFileName );
	poImageShape( poTexture* tex, int maxWidth = 0, int maxHeight = 0 );
	poImageShape( poTexture* tex, int maxWidth, int maxHeight, const poColor& fillColor );
	
	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poRectShape"; };
	virtual void		init();
	
	void	reshape( float W, float H );
	float	getWidth()  { return rectWidth; };
	float	getHeight()  { return rectHeight; };
	
protected:
	float	rectWidth, rectHeight;
	void	construct( poTexture* tex, int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor);
};

/*class poArcShape : public poShape2D
{
public:
	poArcShape( float innerRadius, float outerRadius, float startAngle, float endAngle );

	virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	virtual char*		getClassName() { return "poOvalShape"; };
	virtual void		init();

private:
	void construct(  float innerRadius, float outerRadius, float startAngle, float endAngle );
};*/


#endif

