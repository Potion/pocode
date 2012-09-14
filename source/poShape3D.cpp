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
//  poShape3D.cpp
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poShape3D.h"
#include "poOpenGLState.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poTexture.h"

#include <boost/foreach.hpp>

poVertex3D      poShape3D::errorVertex;
poTriangle3D    poShape3D::errorTriangle;

poShape3D::poShape3D()
{
    useVertexNormals = false;
    useVertexColors = false;
    useVertexTextureCoords = false;
    
    fillColor = poColor::white;
    fillEnabled = true;
    strokeColor = poColor::grey;
    strokeWidth = 0;
    pointColor = poColor::green;
    pointSize = 0;
}


void poShape3D::draw()
{
	po::disableTexture();
	
	glEnableVertexAttribArray(0);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].position.x );
    
    // load vertex texture coordinates (optional)
    if ( useVertexTextureCoords ) {
    }

    // load vertex colors (optional)
    if ( useVertexColors ) {
    }

    // load vertex normals (optional)
    if ( useVertexNormals ) {
		glEnableVertexAttribArray(1);
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].normal.x );
    }

	po::use3DShader();
	po::updateActiveShader();
	
    // draw triangle list (a list of vertex indices)
    if ( fillEnabled )
    {
		po::setColor(fillColor);
        glDrawElements( GL_TRIANGLES, triangleList.size()*3, GL_UNSIGNED_INT, &triangleList[0].vertexIndexSet[0] );
    }
    
    // draw stroke
    if ( strokeWidth > 0 )
    {
		po::setColor(strokeColor);
		po::setLineWidth(strokeWidth);
		
        // Draw each triangle separately. In theory, this could be done with glDrawElements, but it's not working.
        // Also, this draws edges that appear in two triangles twice.
        for( uint i=0; i<triangleList.size(); i++ )
            glDrawElements( GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangleList[i].vertexIndexSet[0] );
    } 

    // draw points
    if ( pointSize > 0 )
    {
		po::setColor(pointColor);
        glPointSize(pointSize);
        glDrawArrays(GL_POINTS, 0, vertexList.size());
    }

    
    /*glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LEQUAL);
     glClear(GL_DEPTH_BUFFER_BIT);
     poBasicRenderer::get()->setFor3D();
     glDisable(GL_DEPTH_TEST);*/
}


int poShape3D::addVertex()
{
    return addVertex( poPoint(), poPoint(), poPoint(), poColor() );
}

int poShape3D::addVertex( poPoint pos ) 
{
    return addVertex( pos, poPoint(), poPoint(), poColor() );
}

int poShape3D::addVertex( poPoint pos, poPoint texCoords ) 
{
    return addVertex( pos, texCoords, poPoint(), poColor() );
}

int poShape3D::addVertex( poPoint pos, poPoint texCoords, poPoint normal ) 
{
    return addVertex( pos, texCoords, normal, poColor() );
}

int poShape3D::addVertex( poPoint pos, poPoint texCoords, poPoint normal, poColor color ) 
{
    poVertex3D V;
    
    V.position = pos;
    V.normal = normal;
    V.textureCoords = texCoords;
    V.color = color;
    
    vertexList.push_back( V );
    return vertexList.size()-1;
}

poVertex3D&         poShape3D::getVertex( int N ) 
{
    if ( N < 0 || N >= (int)vertexList.size() )
    {
        printf("ERROR: getVertex out of bounds (%d of %d)\n", N, (int)vertexList.size() );
        return poShape3D::errorVertex;
    }
    
    return vertexList[N];
}


int poShape3D::addTriangle()
{
    poTriangle3D T;
    triangleList.push_back( T );
    triangleSizeVector.push_back(3);
    return triangleList.size()-1;
}


int poShape3D::addTriangle( int vertexIndexA, int vertexIndexB, int vertexIndexC ) 
{
    poTriangle3D T;
    T.set( vertexIndexA, vertexIndexB, vertexIndexC );
    triangleList.push_back( T );
    return triangleList.size()-1;
}


poTriangle3D&       poShape3D::getTriangle( int N ) 
{    
    if ( N < 0 || N >= (int)triangleList.size() )
    {
        printf("ERROR: getTriangle out of bounds (%d of %d)\n", N, (int)triangleList.size() );
        return poShape3D::errorTriangle;
    }
    
    return triangleList[N];
}

void        poShape3D::calculateNormals()
{
    // zero out all normals
    for( uint i=0; i<vertexList.size(); i++ )
        vertexList[i].normal.set( 0,0,0 );
    
    // add in normals for each face
    for( uint i=0; i<triangleList.size(); i++ )
    {
        poTriangle3D& T = triangleList[i];
        
        int indexA = T.vertexIndexSet[0];
        int indexB = T.vertexIndexSet[1];
        int indexC = T.vertexIndexSet[2];
        
        poPoint A = vertexList[ indexA ].position;
        poPoint B = vertexList[ indexB ].position;
        poPoint C = vertexList[ indexC ].position;
        
        poPoint AB = B - A;
        poPoint AC = C - A;
        poPoint normal = AB.getCross( AC );
        normal.normalize();
        
        vertexList[ indexA ].normal += normal;
        vertexList[ indexB ].normal += normal;
        vertexList[ indexC ].normal += normal;
    }
    
    // average (normalize) all normals
    for( uint i=0; i<vertexList.size(); i++ )
        vertexList[i].normal.normalize();
}

// localize will convert global to local first
// otherwise, point is assumed to be local
bool poShape3D::pointInside(poPoint point, bool localize ) {
	if(!visible)
		return false;
	
    // DO POINT INSIDE TEST FOR 2D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_2D )
    {
        // presently, no text defined for 2D camera
        return false;
    }
    
    // DO POINT INSIDE TEST FOR 3D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_3D )
    {
        if(localize)
            point.y = po::getWindowHeight() - point.y;
        
        for( uint i=0; i<triangleList.size(); i++ )
        {
            int indexA = triangleList[i].vertexIndexSet[0];
            int indexB = triangleList[i].vertexIndexSet[1];
            int indexC = triangleList[i].vertexIndexSet[2];
            
            poPoint pointA = vertexList[indexA].position;
            poPoint pointB = vertexList[indexB].position;
            poPoint pointC = vertexList[indexC].position;
            
            if ( pointInTriangle3D( point, getMatrixSet(), pointA, pointB, pointC ))
                return true;
        }
    }
    
	return false;
}

