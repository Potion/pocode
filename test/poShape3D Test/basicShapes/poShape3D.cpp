//
//  poShape3D.cpp
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poShape3D.h"

#include "poOpenGLState.h"
#include "poBasicRenderer.h"

// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
// http://www.opengl.org/wiki/VBO_-_just_examples


poVertex3D      poShape3D::errorVertex;
poTriangle3D    poShape3D::errorTriangle;

poShape3D::poShape3D()
{
    /*poVertex3D V;
    
    addVertex( poPoint( 0,0,0) );
    addVertex( poPoint(100,0,0) );
    addVertex( poPoint(100,200,0) );

    addVertex( poPoint(200,200,0) );
    addVertex( poPoint(200,300,0) );
    addVertex( poPoint(300,300,0) );
    
    addTriangle( 0,1,2 );
    addTriangle( 3,4,5 );*/
    
    useVertexNormals = false;
    useVertexColors = false;
    useVertexTextureCoords = false;
}


void poShape3D::draw()
{
    poOpenGLState *ogl = poOpenGLState::get();
    ogl->color = poColor::red;
	ogl->setTexture(po::TextureState());
    
    po::VertexState vert;
    vert.enableAttrib(0).disableAttrib(1).disableAttrib(2).disableAttrib(3);
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].position.x ); 
    
//    if ( useVertexTextureCoords ) {
//        vert.enableAttrib(1);
//        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].textureCoords.x ); 
//    }
//
//    if ( useVertexColors ) {
//        vert.enableAttrib(2);
//        glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].textureCoords.x );
//    }

    if ( useVertexNormals ) {
        vert.enableAttrib(1);
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), &vertexList[0].normal.x );     
    }
    
    ogl->setVertex(vert);
    

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    poBasicRenderer::get()->setFor3D();

    //last argument is starting point of the indices
    glDrawElements( GL_TRIANGLES, triangleList.size()*3, GL_UNSIGNED_INT, &triangleList[0].vertexIndexSet[0] );
    
    glDisable(GL_DEPTH_TEST);
    
    // extern void glMultiDrawElements (GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
    // int* indexLocation = &triangleList[0].vertexIndexSet[0];
    // glMultiDrawElements( GL_LINE_LOOP, &triangleSizeVector[0], GL_UNSIGNED_INT, (const GLvoid **)indexLocation, triangleSizeVector.size() );
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
    if ( N < 0 || N >= vertexList.size() )
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
    if ( N < 0 || N >= triangleList.size() )
    {
        printf("ERROR: getTriangle out of bounds (%d of %d)\n", N, (int)triangleList.size() );
        return poShape3D::errorTriangle;
    }
    
    return triangleList[N];
}

void        poShape3D::calculateNormals()
{
    // zero out all normals
    for( int i=0; i<vertexList.size(); i++ )
        vertexList[i].normal.set( 0,0,0 );
    
    // add in normals for each face
    for( int i=0; i<triangleList.size(); i++ )
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
        poPoint normal = AB.cross( AC );
        normal.normalize();
        
        vertexList[ indexA ].normal += normal;
        vertexList[ indexB ].normal += normal;
        vertexList[ indexC ].normal += normal;
    }
    
    // average (normalize) all normals
    for( int i=0; i<vertexList.size(); i++ )
        vertexList[i].normal.normalize();
}


/*

void poShape3D::draw()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //0 and 3 are the first and last vertices
    //glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //glDrawRangeElements may or may not give a performance advantage over glDrawElements
}


void poShape3D::construct()
{
    // set buffer data
    glGenBuffers(1, (GLuint*) &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poVertex3D)*vertexList.size(), &vertexList[0].position.x, GL_STATIC_DRAW);
    
    glGenBuffers(1, (GLuint*) &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*triangleList.size(), &triangleList[0], GL_STATIC_DRAW);
    
    
    // setup vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    
    glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(poVertex3D), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
    
    glEnableVertexAttribArray(1);    //We like submitting normals on stream 1 for no special reason
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));     //The starting point of normals, 12 bytes away
     
    glEnableVertexAttribArray(2);    //We like submitting texcoords on stream 2 for no special reason
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
    
}
*/
