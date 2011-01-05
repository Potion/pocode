/*
 *  poShape3D.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 5/19/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poObject.h"

#ifndef _PO_SHAPE_3D_H_
#define _PO_SHAPE_3D_H_

class poTexture;

class poVertexInfo
{
public:
	poVertexInfo() {  pointIndex = normalIndex = textureCoordIndex = -1; };
	
	int		pointIndex;
	int		normalIndex;
	int		textureCoordIndex;
};

typedef std::vector<poVertexInfo> poVertexInfoVector;
typedef std::vector<int> intVector;

class poShape3DFace
{
public:
	poShape3DFace() { };
	
	poVertexInfoVector	vertexInfoList;
	poPoint				normal;
	poColor				color;
};

typedef std::vector<poShape3DFace> poShape3DFaceVector;

class poShape3D :  public poObject
{
public:
	poShape3D();
	virtual ~poShape3D();
	
	virtual poObject*	duplicate( bool copyChildren=false, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poShape3D"; };
		
	virtual void	init();
	virtual void	draw();
	void			drawFaces();
	void			drawWireframe();
	void			drawVertices();
	virtual void	debugDraw();
	
	int				addPoint( float x, float y, float z ) { return addPoint( poPoint(x,y,z) ); };
	int				addPoint( poPoint P );
	int				addNormal( poPoint P );
	int				addTextureCoord( poPoint P );
	int				makeNewFace();
	void			addVertexToFace( int pointIndex, int faceIndex );
	void			addVertexToFace( int pointIndex, int normalIndex, int textureCoordIndex, int faceIndex );
	poPoint&		getFacePoint( int faceIndex, int N );

	virtual bool	pointInside(float x, float y);
	virtual void	calculateNormals();
	void			shiftAllPoints( poPoint P );

	virtual void placeTexture(poTexture* tex, poTextureScaleOption scaleOption=PO_TEX_NO_FIT, poHoriOrientation halign=PO_ALIGN_LEFT, poVertOrientation valign=PO_ALIGN_TOP);
	
	void			makeCallList();

	poPointVector			pointList;
	poPointVector			normalList;
	poPointVector			textureCoordList;
	
	poShape3DFaceVector		faceList;
	poColor					fillColor, strokeColor;
	
	bool			enableFill, enablePoints;
	int				strokeWidth;
	bool			useFaceNormals, useFaceColors;
	poPoint			textureScale;
	int				callListID;
	
	GLenum			fillDrawStyle;
	GLenum			strokeDrawStyle;
};

#endif

