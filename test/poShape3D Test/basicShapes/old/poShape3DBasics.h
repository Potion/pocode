//
//  poShape3DBasics.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/24/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_SHAPE3D_BASICS_H
#define PO_SHAPE3D_BASICS_H

#include "poShape3D.h"
#include "poShape2D.h"

class poMesh3D : public poShape3D
{
public:

	poMesh3D();
	poMesh3D( int rows, int columns );
	
	
	void	construct( int rows, int columns );
	int		getMeshVertexIndex( int x, int y );
	
	int		numRows, numColumns;
};


class poPlane3D : public poMesh3D
{
public:
	poPlane3D();
	poPlane3D( int rows, int columns, float w, float h );

	void	construct( int rows, int columns, float w, float h );
	
	float	width, height;
};

class poSphere3D : public poMesh3D
{
public:
	poSphere3D();
	poSphere3D( int rows, int columns, float radius );
	
	virtual void		calculateNormals();
		
	void	construct( int rows, int columns, float radius );

	
	float	radius;
};


class poCube3D : public poShape3D
{
public:
	poCube3D();
	
	void	construct();
};


class poObjMaterial
{
public:
	std::string		name;
	poColor			diffuseColor;
};

typedef std::vector<poObjMaterial> poObjMaterialVector;


class poOBJShape3D : public poShape3D
{
public:
	poOBJShape3D();
	
	void	readObjFile( char* fileName );	
	void	readObjMtlFile( char* fileName );
	
	poObjMaterial*	getMaterialWithName( std::string matName );
	
	poObjMaterialVector		materialList;
};


class poShape3DMaker
{
public:
	static poShape3D* makeShape3DfromShape2D( poShape2D* S );
	static poShape3D* makeExtrusionFromShape2D( poShape2D* S, float Depth, bool closeShape, bool makeEndCaps );
	static poMesh3D*  makeRevolutionFromShape2D( poShape2D* S, float radialOffset, int numSlices, float startAngle, float endAngle, bool makeEndCaps );
};


#endif
