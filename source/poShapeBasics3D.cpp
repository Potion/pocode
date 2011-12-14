//
//  poShape3DBasics.cpp
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poShapeBasics3D.h"


// ============= poMesh3D ================ //

poMesh3D::poMesh3D( int _numRows, int _numColumns )
{
    numRows = _numRows;
    numColumns = _numColumns;
    
    vertexList.resize( numRows*numColumns );
    
    for( int i=0; i<numRows; i++ )
    {
        for( int j=0; j<numColumns; j++ )
        {
            getVertex( i,j ).position.set( i*10,j*10, 0 );
        }
    }
    
    for( int i=0; i<numRows-1; i++ )
    {
        for( int j=0; j<numColumns-1; j++ )
        {
            int indexA = getVertexIndex(i,j);
            int indexB = getVertexIndex(i+1,j);
            int indexC = getVertexIndex(i,j+1);
            int indexD = getVertexIndex(i+1,j+1);
            addTriangle( indexA, indexB, indexC );
            addTriangle( indexD, indexC, indexB );
        }
    }
    
    calculateNormals();
}


poVertex3D&     poMesh3D::getVertex( int row, int col )
{
    if ( row < 0 || row >= numRows || col < 0 || col >= numColumns )
    {
        printf("ERROR: poMesh3D::getVertex out of bounds (%d,%d) of (%d,%d)\n", row, col, numRows, numColumns );
        return poShape3D::errorVertex;
    }
    
    int index = getVertexIndex(row,col);
    return vertexList[ index ];
}

int             poMesh3D::getVertexIndex( int row, int col )
{
    if ( row < 0 || row >= numRows || col < 0 || col >= numColumns )
    {
        printf("ERROR: poMesh3D::getVertexIndex out of bounds (%d,%d) of (%d,%d)\n", row, col, numRows, numColumns );
        return -1;
    }
    
    return (row*numColumns + col);
}

void            poMesh3D::placeTexture( poTexture* tex )
{
    texture = tex;
    
    for( int i=0; i<numRows; i++ )
    {
        for( int j=0; j<numColumns; j++ )
        {
            float U = (float)i / (float)numRows;
            float V = 1.f - (float)j / (float)numColumns; 
            getVertex( i,j ).textureCoords.set( U,V,0 );
        }
    }
}

// ============= poSphere3D ================ //

poSphere3D::poSphere3D( int _numRows, int _numColumns, float _radius ) : poMesh3D(_numRows,_numColumns)
{
    radius = _radius;
    
	float dA =  360.f / (float)(numColumns-1);
	float dB =  180.f / (float)(numRows-1);
    
	for( int i=0; i<numRows; i++ )
	{
		for( int j=0; j<numColumns; j++ )
		{
			int N = getVertexIndex(i,j);
            if ( N==-1 || N < 0 || N >= (int)vertexList.size() )
            {
                printf("ERROR\n");
                continue;
            }
			float A = -1.f * dA * j;
			float B = 90 - dB * i;
			
			vertexList[N].position.set( cos_deg(B)*cos_deg(A)*radius, sin_deg(B)*radius, cos_deg(B)*sin_deg(A)*radius );
		}
	}
    
	calculateNormals();
}

void	poSphere3D::calculateNormals()
{
    // set sphere normals based upon position relative to center
    for( uint i=0; i<vertexList.size(); i++ )
    {
        vertexList[i].normal = vertexList[i].position; 
        vertexList[i].normal *= -1.0;
        vertexList[i].normal.normalize();
    }
}


// ============= poCube3D ================ //

poCube3D::poCube3D( float W, float H, float D )
{
    cubeWidth = W;
    cubeHeight = H;
    cubeDepth = D;
    
    // front vertices
    int FTL = addVertex( poPoint(0,0,0) );
    int FTR = addVertex( poPoint(W,0,0) );
    int FBR = addVertex( poPoint(W,H,0) );
    int FBL = addVertex( poPoint(0,H,0) );
    
    // back vertices
    int BTL = addVertex( poPoint(0,0,D) );
    int BTR = addVertex( poPoint(W,0,D) );
    int BBR = addVertex( poPoint(W,H,D) );
    int BBL = addVertex( poPoint(0,H,D) );
    
    // front face
    addTriangle( FTL, FTR, FBR );
    addTriangle( FBR, FBL, FTL );
    
    // back face
    addTriangle( BTL, BTR, BBR );
    addTriangle( BBR, BBL, BTL );
    
    // left face
    addTriangle( FTL, BTL, FBL );
    addTriangle( BBL, FBL, BTL );
    
    // right face
    addTriangle( FTR, BTR, FBR );
    addTriangle( BBR, FBR, BTR );
    
    // top face
    addTriangle( FTL, FTR, BTL );
    addTriangle( BTR, FTR, BTL );
    
    // bottom face
    addTriangle( FBL, FBR, BBL );
    addTriangle( BBR, FBR, BBL );
    
    calculateNormals();
}









