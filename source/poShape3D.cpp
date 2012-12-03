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
//  Shape3D.cpp
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
namespace po {
    Vertex3D      Shape3D::errorVertex;
    Triangle3D    Shape3D::errorTriangle;
    
    Shape3D::Shape3D() {
        useVertexNormals = false;
        useVertexColors = false;
        useVertexTextureCoords = false;
        
        fillColor = Color::white;
        fillEnabled = true;
        strokeColor = Color::grey;
        strokeWidth = 0;
        pointColor = Color::green;
        pointSize = 0;
    }
    
    
    //------------------------------------------------------------------------
    void Shape3D::draw() {
        po::disableTexture();
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), &vertexList[0].position.x );
        
        // load vertex texture coordinates (optional)
        if ( useVertexTextureCoords ) {
        }

        // load vertex colors (optional)
        if ( useVertexColors ) {
        }

        // load vertex normals (optional)
        if ( useVertexNormals ) {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), &vertexList[0].normal.x );
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
    
    
    
    
    // ----------------------------------------------------------------------------------
    // ================================ Vertices ========================================
    #pragma mark Vertices
    
    //------------------------------------------------------------------------
    int Shape3D::addVertex() {
        return addVertex( Point(), Point(), Point(), Color() );
    }
    
    
    //------------------------------------------------------------------------
    int Shape3D::addVertex( Point pos ) {
        return addVertex( pos, Point(), Point(), Color() );
    }
    
    
    //------------------------------------------------------------------------
    int Shape3D::addVertex( Point pos, Point texCoords ) {
        return addVertex( pos, texCoords, Point(), Color() );
    }
    
    
    //------------------------------------------------------------------------
    int Shape3D::addVertex( Point pos, Point texCoords, Point normal ) {
        return addVertex( pos, texCoords, normal, Color() );
    }
    
    
    //------------------------------------------------------------------------
    int Shape3D::addVertex( Point pos, Point texCoords, Point normal, Color color ) {
        Vertex3D V;
        
        V.position = pos;
        V.normal = normal;
        V.textureCoords = texCoords;
        V.color = color;
        
        vertexList.push_back( V );
        return vertexList.size()-1;
    }
    
    
    //------------------------------------------------------------------------
    Vertex3D& Shape3D::getVertex( int N ) {
        if ( N < 0 || N >= (int)vertexList.size() )
        {
            printf("ERROR: getVertex out of bounds (%d of %d)\n", N, (int)vertexList.size() );
            return Shape3D::errorVertex;
        }
        
        return vertexList[N];
    }
    
    
    
    
    // ----------------------------------------------------------------------------------
    // ================================ Triangles =======================================
    #pragma mark Triangles
    
    //------------------------------------------------------------------------
    int Shape3D::addTriangle() {
        Triangle3D T;
        triangleList.push_back( T );
        triangleSizeVector.push_back(3);
        return triangleList.size()-1;
    }
    
    
    //------------------------------------------------------------------------
    int Shape3D::addTriangle( int vertexIndexA, int vertexIndexB, int vertexIndexC ) {
        Triangle3D T;
        T.set( vertexIndexA, vertexIndexB, vertexIndexC );
        triangleList.push_back( T );
        return triangleList.size()-1;
    }
    
    
    //------------------------------------------------------------------------
    Triangle3D& Shape3D::getTriangle( int N ) {
        if ( N < 0 || N >= (int)triangleList.size() )
        {
            printf("ERROR: getTriangle out of bounds (%d of %d)\n", N, (int)triangleList.size() );
            return Shape3D::errorTriangle;
        }
        
        return triangleList[N];
    }
    
    
    
    
    // ----------------------------------------------------------------------------------
    // ================================ Lighting ========================================
    #pragma mark Lighting
    
    //------------------------------------------------------------------------
    void Shape3D::calculateNormals() {
        // zero out all normals
        for( uint i=0; i<vertexList.size(); i++ )
            vertexList[i].normal.set( 0,0,0 );
        
        // add in normals for each face
        for( uint i=0; i<triangleList.size(); i++ ) {
            Triangle3D& T = triangleList[i];
            
            int indexA = T.vertexIndexSet[0];
            int indexB = T.vertexIndexSet[1];
            int indexC = T.vertexIndexSet[2];
            
            Point A = vertexList[ indexA ].position;
            Point B = vertexList[ indexB ].position;
            Point C = vertexList[ indexC ].position;
            
            Point AB = B - A;
            Point AC = C - A;
            Point normal = AB.getCross( AC );
            normal.normalize();
            
            vertexList[ indexA ].normal += normal;
            vertexList[ indexB ].normal += normal;
            vertexList[ indexC ].normal += normal;
        }
        
        // average (normalize) all normals
        for( uint i=0; i<vertexList.size(); i++ )
            vertexList[i].normal.normalize();
    }
    
    
    //------------------------------------------------------------------------
    // localize will convert global to local first
    // otherwise, point is assumed to be local
    bool Shape3D::pointInside(Point point, bool localize ) {
        if(!visible)
            return false;
        
        // DO POINT INSIDE TEST FOR 2D
        if ( Camera::getCurrentCameraType() == po::CAMERA_2D ) {
            // presently, no text defined for 2D camera
            return false;
        }
        
        // DO POINT INSIDE TEST FOR 3D
        if ( Camera::getCurrentCameraType() == po::CAMERA_3D ) {
            if(localize)
                point.y = po::getWindowHeight() - point.y;
            
            for( uint i=0; i<triangleList.size(); i++ ) {
                int indexA = triangleList[i].vertexIndexSet[0];
                int indexB = triangleList[i].vertexIndexSet[1];
                int indexC = triangleList[i].vertexIndexSet[2];
                
                Point pointA = vertexList[indexA].position;
                Point pointB = vertexList[indexB].position;
                Point pointC = vertexList[indexC].position;
                
                if ( pointInTriangle3D( point, getMatrixSet(), pointA, pointB, pointC ))
                    return true;
            }
        }
        
        return false;
    }
} /* End po namespace */