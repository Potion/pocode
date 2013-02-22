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
//  Shape3D.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#pragma once

/////////////////////////////////////////////////////
//
//	pocode's 3D lighting system is UNDER DEVELOPMENT
//	Check again in future releases of pocode
//
/////////////////////////////////////////////////////

#include "poObject.h"
#include "poTexture.h"

namespace po {
    //Vertex3D
    class Vertex3D {
    public:
        Point position;
        Point normal;
        Point textureCoords;
        Color color;
    };

    //Triangle3D
    class Triangle3D {
    public:
        int     vertexIndexSet[3];
        
        void    set( int A, int B, int C ) { vertexIndexSet[0]=A; vertexIndexSet[1]=B; vertexIndexSet[2]=C; };
    };

    typedef std::vector<Vertex3D>    Vertex3DVector;
    typedef std::vector<Triangle3D>  Triangle3DVector;


    //Shape3D
    class Shape3D : public Object {
    public:
        Shape3D();
        
        virtual void        draw();
        
        int                 addVertex();
        int                 addVertex( Point pos );
        int                 addVertex( Point pos, Point texCoords );
        int                 addVertex( Point pos, Point texCoords, Point normal );
        int                 addVertex( Point pos, Point texCoords, Point normal, Color color );
        
        int                 numVertices() { return vertexList.size(); };
        Vertex3D&         getVertex( int N );
        
        int                 addTriangle();
        int                 addTriangle( int vertexIndexA, int vertexIndexB, int vertexIndexC );
        
        int                 numTriangles() { return triangleList.size(); };
        Triangle3D&       getTriangle( int N );
        
        //void              generateTrianglesFromPolygon( std::vector<Point> pointList );
        
        virtual void        calculateNormals();
        
        virtual bool        pointInside(Point point, bool localize=false);

        // VERTEX PROPERTIES
        bool                useVertexNormals;
        bool                useVertexColors;
        bool                useVertexTextureCoords;
        
        // FILL AND STROKE AND POINTS
        Color             fillColor;
        bool                fillEnabled;
        Color             strokeColor;
        int                 strokeWidth;
        Color             pointColor;
        int                 pointSize;
        
        Texture*			texture;
        
    protected:
        Triangle3DVector  triangleList;
        Vertex3DVector    vertexList;
        
        std::vector<int>    triangleSizeVector;
        
        int                 vertexBufferID, indexBufferID;
        
        static Vertex3D   errorVertex;
        static Triangle3D errorTriangle;
        
    };
} /* End po namespace */