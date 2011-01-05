/*
 *  poShape2D.h
 *  potionApp
 *
 *  Created by Jared Schiffman on 9/14/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */


#ifndef _PO_SHAPE_2D_H_
#define _PO_SHAPE_2D_H_

#include "poObject.h"
#include "poTexture.h"

class poShape2D :  public poObject
{
public:
	poShape2D();
	virtual ~poShape2D();
	
	virtual poObject*	duplicate( bool copyChildren=false, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poShape2D"; };
		
	virtual void init();
	virtual void draw();
	virtual void debugDraw();
	
	void addPoint(float x, float y);
	void addPoint(const poPoint& p);
	void addPoint(const poPoint& p, const poColor& c);
	void setPoint( int N, poPoint& P ) { if ( N < pointList.size() ) pointList[N] = P; };
	
	virtual void setOrigin(poHoriOrientation hor, poVertOrientation vor);
	void		 updateOrigin();
	
	virtual void calculateBounds();
	virtual void calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox );
	virtual bool pointInside(float x, float y);
	bool		 pointInside3D(float x, float y);
	bool		 pointInTriangle3D( poPoint P, poPoint A, poPoint B, poPoint C );
	
	void placeTexture(poTexture* tex, poTextureScaleOption scaleOption=PO_TEX_NO_FIT, poHoriOrientation halign=PO_ALIGN_LEFT, poVertOrientation valign=PO_ALIGN_TOP);
	void transformTexture(float transX, float transY, float scale, float rotation);
	
	bool enableFill;
	int strokeWidth;
	poColor fillColor;
	poColor strokeColor;

	GLenum fillDrawStyle;
	GLenum strokeDrawStyle;
	bool useThickStrokeMethod;
	poStrokePlacementOption strokePlacement;
	
	bool usePointInside3D;
	bool includeChildrenInBounds;
	
//protected:
	void drawFillStandard();
	void drawFillWithTexture();
	void drawFillWithColorPoints();
	void drawFillWithColorPointsAndTexture();
	void drawStroke();
	
	void shiftAllPoints(const poPoint& p);

	void makeCallList();
	void doTessellation();

	poPointVector pointList;
	poColorVector pointColorList;
	poPointVector texCoordList;
	
	poHoriOrientation hOrigin;
	poVertOrientation vOrigin;

	int callListID;
};

typedef std::vector<poShape2D*> poShapeVector;

#endif