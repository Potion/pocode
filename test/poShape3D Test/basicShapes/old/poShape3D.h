//
//  poShape3D.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/24/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_SHAPE_3D_H
#define PO_SHAPE_3D_H

#include "poObject.h"

class poVertexInfo
{
public:
	poVertexInfo() {  pointIndex = normalIndex = textureCoordIndex = -1; };
	
	int		pointIndex;
	int		normalIndex;
	int		textureCoordIndex;
};


typedef std::vector<poVertexInfo> poVertexInfoVector;


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
	
	virtual void            draw();
	void                    drawFaces();
	void                    drawWireframe();
	void                    drawVertices();
	//virtual void          debugDraw();
	
	int                     addPoint( float x, float y, float z ) { return addPoint( poPoint(x,y,z) ); };
	int                     addPoint( poPoint P );
	int                     addNormal( poPoint P );
	int                     addTextureCoord( poPoint P );
    
	int                     makeNewFace();
	void                    addVertexToFace( int pointIndex, int faceIndex );
	void                    addVertexToFace( int pointIndex, int normalIndex, int textureCoordIndex, int faceIndex );
	poPoint&                getFacePoint( int faceIndex, int N );
    
	virtual bool            pointInside(float x, float y);
	virtual void            calculateNormals();
	void                    shiftAllPoints( poPoint P );
    
	//virtual void        placeTexture(poTexture* tex, poTextureScaleOption scaleOption=PO_TEX_NO_FIT, poHoriOrientation halign=PO_ALIGN_LEFT, poVertOrientation valign=PO_ALIGN_TOP);
	
	void                    makeCallList();
    
    std::vector<poPoint>	pointList;
	std::vector<poPoint>	normalList;
	std::vector<poPoint>	textureCoordList;
	
	poShape3DFaceVector		faceList;
	poColor					fillColor, strokeColor;
	
	bool                    enableFill, enablePoints;
	int                     strokeWidth;
	bool                    useFaceNormals, useFaceColors;
	poPoint                 textureScale;
	int                     callListID;
	
	GLenum                  fillDrawStyle;
	GLenum                  strokeDrawStyle;
};


#endif
