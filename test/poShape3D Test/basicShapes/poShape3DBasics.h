//
//  poShape3DBasics.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_SHAPE_3D_BASICS_H
#define PO_SHAPE_3D_BASICS_H

#include "poShape3D.h"

class poMesh3D : public poShape3D
{
public:
    poMesh3D( int _numRows, int _numColumns );
    
    poVertex3D&     getVertex( int row, int col );
    int             getVertexIndex( int row, int col );

    void            placeTexture( poTexture tex );
    
    int     numRows, numColumns;
};



class poSphere3D : public poMesh3D
{
public:
    poSphere3D( int _numRows, int _numColumns, float radius );
    
    virtual void        calculateNormals();
    
    float   radius;
};


class poCube3D : public poShape3D
{
public:
    poCube3D( float W, float H, float D );

    float cubeWidth, cubeHeight, cubeDepth;
};


#endif
