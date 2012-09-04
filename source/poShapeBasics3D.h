/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  poShape3DBasics.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#pragma once


#include "poShape3D.h"
#include "poTexture.h"

class poMesh3D : public poShape3D
{
public:
    poMesh3D( int _numRows, int _numColumns );
    
    poVertex3D&     getVertex( int row, int col );
    int             getVertexIndex( int row, int col );

    void            placeTexture( poTexture* tex );
    
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
