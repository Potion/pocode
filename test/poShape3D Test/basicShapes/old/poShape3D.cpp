//
//  poShape3D.cpp
//  basicShapes
//
//  Created by Jared Schiffman on 10/24/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poShape3D.h"
#include "SimpleDrawing.h"


poShape3D::poShape3D()
{
	enableFill = true;
	enablePoints = false;
	strokeWidth = 0;
	useFaceNormals = false;
	useFaceColors = false;
	textureScale.set(1,1,1);
	callListID = -1;
	
	fillDrawStyle = GL_POLYGON;
	strokeDrawStyle = GL_LINE_LOOP;
}

poShape3D::~poShape3D()
{
}


void poShape3D::draw()
{
	po::setColor(fillColor, appliedAlpha());
	
	if(callListID != -1)
		glCallList(callListID);
	else if ( enableFill )
		drawFaces();
	else if ( strokeWidth != 0 )
		drawWireframe();
	else if ( enablePoints )
		drawVertices();
}


void poShape3D::drawFaces()
{
	po::setColor(fillColor, appliedAlpha());
	
	// for every face
	for( int f=0; f<faceList.size(); f++ )
	{
		poShape3DFace& F = faceList[f];
		
		// draw each face, vertex by vertex
		glBegin( fillDrawStyle );
		
		if ( useFaceNormals )
            glNormal3fv( (float*) &F.normal );
		if ( useFaceColors )
			glColor4f( F.color.R, F.color.G, F.color.B, appliedAlpha());
        
		for( int v=0; v<F.vertexInfoList.size(); v++ )
		{
			int	normalIndex = F.vertexInfoList[v].normalIndex;
			if ( normalIndex != -1 )
				glNormal3fv( (float*) &normalList[ normalIndex ] );
			
			int	texCoordIndex = F.vertexInfoList[v].textureCoordIndex;
			if ( texCoordIndex != -1 )
				glTexCoord2f( textureCoordList[ texCoordIndex ].x*textureScale.x, textureCoordList[ texCoordIndex ].y*textureScale.y );
			
			int	pointIndex = F.vertexInfoList[v].pointIndex;
			if ( pointIndex != -1 )
				glVertex3fv( (float*) &pointList[ pointIndex ] );
		}
		glEnd();
	}
}

void poShape3D::drawWireframe()
{
	glColor4f( strokeColor.R, strokeColor.G, strokeColor.B, appliedAlpha() );
	glLineWidth( strokeWidth );
	
	// for every face  
	for( int f=0; f<faceList.size(); f++ )
	{
		poShape3DFace& F = faceList[f];
		
		// draw each face, vertex by vertex
		glBegin( strokeDrawStyle );
        
		for( int v=0; v<F.vertexInfoList.size(); v++ )
		{
			int	pointIndex = F.vertexInfoList[v].pointIndex;
			if ( pointIndex != -1 )
				glVertex3fv( (float*) &pointList[ pointIndex ] );
		}
		glEnd();
	}
	
	glLineWidth( 1 );
}

void poShape3D::drawVertices()
{
    po::setColor( fillColor, appliedAlpha() );
    po::drawPoints( pointList, GL_POINTS );
}


int	poShape3D::addPoint( poPoint P )
{
	pointList.push_back( P );
	return pointList.size()-1;
}

int	poShape3D::addNormal( poPoint P )
{
	normalList.push_back( P );
	return normalList.size()-1;
}

int	poShape3D::addTextureCoord( poPoint P )
{
	textureCoordList.push_back( P );
	return textureCoordList.size()-1;
}

int	poShape3D::makeNewFace()
{
	poShape3DFace F;
	faceList.push_back( F );
	return faceList.size()-1;
}

void poShape3D::addVertexToFace( int pointIndex, int faceIndex )
{
	if ( faceIndex < 0 || faceIndex >= faceList.size() )
	{
		printf("ERROR: poShape3D::addVertexToFace faceIndex out of bounds\n");
		return;
	}
	
	addVertexToFace( pointIndex, -1, -1, faceIndex );
}

void poShape3D::addVertexToFace( int pointIndex, int normalIndex, int textureCoordIndex, int faceIndex )
{
	if ( faceIndex < 0 || faceIndex >= faceList.size() )
	{
		printf("ERROR: poShape3D::addVertexToFace faceIndex out of bounds\n");
		return;
	}
	
	poVertexInfo vertex;
	vertex.pointIndex = pointIndex;
	vertex.normalIndex = normalIndex;
	vertex.textureCoordIndex = textureCoordIndex;
	faceList[faceIndex].vertexInfoList.push_back( vertex );
}


poPoint	errorPoint3D;

poPoint&	poShape3D::getFacePoint( int faceIndex, int N )
{
	if ( faceIndex < 0 || faceIndex >= faceList.size() )
	{
		printf("ERROR: poShape3D::getFaceVertex faceIndex out of bounds\n");
		return errorPoint3D;
	}
	
	poShape3DFace& F = faceList[faceIndex];
	if ( N < 0 || N >= F.vertexInfoList.size() )
	{
		printf("ERROR: poShape3D::getFaceVertex N out of bounds\n");
		return errorPoint3D;
	}
	
	int pointIndex = F.vertexInfoList[N].pointIndex;
	return pointList[ pointIndex ];
}


bool	poShape3D::pointInside(float x, float y)
{
	/*if( !visible || matrixSet.isZero() ) {
		return false;
	}
	
    poPoint P;
    P.set( x, y, 0 );
    
	// for every face
	for( int f=0; f<faceList.size(); f++ )
	{
		poShape3DFace& F = faceList[f];
		
		// go through each face, vertex by vertex
		for( int v=1; v<F.vertexInfoList.size()-1; v++ )
		{
			int	pointIndex0 = F.vertexInfoList[0].pointIndex;
			int	pointIndex1 = F.vertexInfoList[v].pointIndex;
			int	pointIndex2 = F.vertexInfoList[v+1].pointIndex;
			poPoint V0 = pointList[ pointIndex0 ];
			poPoint V1 = pointList[ pointIndex1 ];
			poPoint V2 = pointList[ pointIndex2 ];
			localToGlobal( V0 );
			localToGlobal( V1 );
			localToGlobal( V2 );
			V0.z = V1.z = V2.z = 0;
			if ( pointInTriangle( P, V0, V1, V2 ) )
				return true;
		}
	}*/
    
    return false;
}


void poShape3D::shiftAllPoints( poPoint P )
{
	for( int i=0; i<pointList.size(); i++ )
		pointList[i] += P;
}


void poShape3D::calculateNormals()
{
	// zero out normals
	for( int i=0; i<normalList.size(); i++ )
		normalList[i].set( 0,0,0 );
    
	// for every face
	for( int f=0; f<faceList.size(); f++ )
	{
		poShape3DFace& F = faceList[f];
		if ( F.vertexInfoList.size() < 3 )
			continue;
        
		// get 3 points to define plane	
		int	pointIndex0 = F.vertexInfoList[0].pointIndex;
		int	pointIndex1 = F.vertexInfoList[1].pointIndex;
		int	pointIndex2 = F.vertexInfoList[2].pointIndex;
		
		poPoint A = pointList[ pointIndex0 ];
		poPoint B = pointList[ pointIndex1 ];
		poPoint C = pointList[ pointIndex2 ];
		
		poPoint dV1 = B - A;
		poPoint dV2 = C - A;
		
		F.normal = dV1.cross( dV2 );
		F.normal.normalize();
		
		// accumulate normals for all points in face
		for( int v=0; v<F.vertexInfoList.size(); v++ )
		{
			int	normalndex = F.vertexInfoList[v].normalIndex;
			if ( normalndex != -1 )
				normalList[ normalndex ] += F.normal;
		}
	}
	
	// normalize (average) normals
	for( int i=0; i<normalList.size(); i++ )
		normalList[i].normalize();
    
	useFaceNormals = true;
}


/*void poShape3D::placeTexture(poTexture* tex, poTextureScaleOption scaleOption, poHoriOrientation halign, poVertOrientation valign )
{
	printf("poShape3D::placeTexture\n");
    
#ifdef WIN32
	textureScale.set( 1,1,1 );
#else
	textureScale.set( tex->width, tex->height );
#endif
    
	printf("texture scale %f %f\n", textureScale.x, textureScale.y );
}*/


void poShape3D::makeCallList()
{
	if(callListID != -1)
		printf("WARNING: poShape2D->makeCallList: over-writing call list\n");
	
	callListID = -1;
	int tmp = glGenLists(1);
	glNewList(tmp, GL_COMPILE);
	draw();
	glEndList();
	
	callListID = tmp;
}



