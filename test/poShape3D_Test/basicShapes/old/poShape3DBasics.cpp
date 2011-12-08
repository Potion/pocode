//
//  poShape3DBasics.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/24/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "poShape3DBasics.h"


// ===================== poMesh3D ===================== //

poMesh3D::poMesh3D()
{
	numRows = 0;
	numColumns = 0;
}


poMesh3D::poMesh3D( int rows, int columns )
{
	construct( rows, columns );
}

/*poObject*	poMesh3D::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poMesh3D* obj = (poMesh3D*) copyIntoObject;
	if ( obj==NULL )
		obj = new poMesh3D();
	
	// set poShape2D specific properties
	obj->numRows = numRows;
	obj->numColumns = numColumns;
	
	// set poObject proeperties
	poShape3D::duplicate( copyChildren, obj );
	return obj;
}*/


void	poMesh3D::construct( int rows, int columns )
{
	if ( rows==0 || columns==0 )
		return;
	
	pointList.clear();
	normalList.clear();
	textureCoordList.clear();
	faceList.clear();
	
	numRows = rows;
	numColumns = columns;

	for( int j=0; j<numRows; j++ )
	{
		for( int i=0; i<numColumns; i++ )
		{
			addPoint( poPoint(i*10, j*10, 0) );
			addTextureCoord( poPoint((float) i /  (float)(numColumns-1), (float) j /  (float)(numRows-1), 0) );
		}
	}
	
	for( int j=0; j<numRows-1; j++ )
	{
		for( int i=0; i<numColumns-1; i++ )
		{
			// addVertexToFace( pInd-1, nInd-1, tInd-1, Face );
			// int pointIndex, int normalIndex, int textureCoordIndex, int faceIndex )
			
			int F1 = makeNewFace();
			addVertexToFace( getMeshVertexIndex(i,j), -1, getMeshVertexIndex(i,j), F1 );			// point and tex coords have same index
			addVertexToFace( getMeshVertexIndex(i+1,j), -1, getMeshVertexIndex(i+1,j), F1 );
			addVertexToFace( getMeshVertexIndex(i,j+1), -1, getMeshVertexIndex(i,j+1), F1 );
			
			int F2 = makeNewFace();
			addVertexToFace( getMeshVertexIndex(i+1,j+1), -1, getMeshVertexIndex(i+1,j+1), F2 );	// point and tex coords have same index
			addVertexToFace( getMeshVertexIndex(i,j+1), -1,  getMeshVertexIndex(i,j+1), F2 );
			addVertexToFace( getMeshVertexIndex(i+1,j), -1,  getMeshVertexIndex(i+1,j), F2 );
		}
	}
}

int		poMesh3D::getMeshVertexIndex( int x, int y )
{
	return (y*numColumns+x);
}


// ===================== poPlane3D ===================== //

poPlane3D::poPlane3D() : poMesh3D()
{
	construct( 4,4, 100,100 );
}


/*poObject*	poPlane3D::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poPlane3D* obj = (poPlane3D*) copyIntoObject;
	if ( obj==NULL )
		obj = new poPlane3D();
	
	// set poPlane3D specific properties
	obj->width = width;
	obj->height = height;
	
	// set poObject proeperties
	poMesh3D::duplicate( copyChildren, obj );
	return obj;
}*/

	
void	poPlane3D::construct( int rows, int columns, float w, float h  )
{
	if ( rows==0 || columns==0 )
		return;
	
	poMesh3D::construct( rows, columns );
	width = w;
	height = h;
	
	float dX = width / (float)(numColumns-1);
	float dY = height / (float)(numRows-1);
		
	for( int j=0; j<numRows; j++ )
	{
		for( int i=0; i<numColumns; i++ )
		{
			int N = getMeshVertexIndex(i,j);
			pointList[N].set( i*dX, j*dY, 0 );
		}
	}
	
	calculateNormals();
}
	

// ===================== poSphere3D ===================== //

poSphere3D::poSphere3D() : poMesh3D()
{
	construct( 5,10, 100 );
}

poSphere3D::poSphere3D( int rows, int columns, float radius )
{
	construct( rows, columns, radius );
}

/*poObject*	poSphere3D::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poSphere3D* obj = (poSphere3D*) copyIntoObject;
	if ( obj==NULL )
		obj = new poSphere3D();
	
	// set poPlane3D specific properties
	obj->radius = radius;
	
	// set poObject proeperties
	poMesh3D::duplicate( copyChildren, obj );
	return obj;
}*/
	
void	poSphere3D::construct( int rows, int columns, float radius  )
{
	if ( rows==0 || columns==0 )
		return;
	
	poMesh3D::construct( rows, columns );
	this->radius = radius;

	float dA =  360.0 / (float)(numColumns-1);
	float dB =  180.0 / (float)(numRows-1);
		
	for( int j=0; j<numRows; j++ )
	{
		for( int i=0; i<numColumns; i++ )
		{
			int N = getMeshVertexIndex(i,j);
			float A = -1.0 * dA * i;
			float B = 90 - dB * j;
			
			pointList[N].set(  cos_deg(B)*cos_deg(A)*radius, sin_deg(B)*radius, cos_deg(B)*sin_deg(A)*radius );
		}
	}
		
	calculateNormals();
}


void	poSphere3D::calculateNormals()
{
	// do regular calculation first to get face normals
	poShape3D::calculateNormals();
	
	/*printf("using special sphere normals\n");
	for( int i=0; i<vertexList.size(); i++ )
	{
		vertexList[i].normal = vertexList[i].position; 
		vertexList[i].normal *= -1.0;
		vertexList[i].normal.normalize();
	}
	
	useFaceNormals = false;*/
}

// ===================== poCube3D ===================== //


poCube3D::poCube3D() : poShape3D()
{
	construct();
}


/*poObject*	poCube3D::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poCube3D* obj = (poCube3D*) copyIntoObject;
	if ( obj==NULL )
		obj = new poCube3D();
	
	
	// set poObject proeperties
	poShape3D::duplicate( copyChildren, obj );
	return obj;
}*/

	
void	poCube3D::construct()
{
	float S = 10.0;
	
	// front
	int F = makeNewFace();
	addVertexToFace( addPoint(0,0,0), F );
	addVertexToFace( addPoint(S,0,0), F );
	addVertexToFace( addPoint(S,S,0), F );
	addVertexToFace( addPoint(0,S,0), F );
	
	// right
	F = makeNewFace();
	addVertexToFace( addPoint(S,0,0), F );
	addVertexToFace( addPoint(S,0,S), F );
	addVertexToFace( addPoint(S,S,S), F );
	addVertexToFace( addPoint(S,S,0), F );
	
	// back
	F = makeNewFace();
	addVertexToFace( addPoint(S,0,S), F );
	addVertexToFace( addPoint(0,0,S), F );
	addVertexToFace( addPoint(0,S,S), F );
	addVertexToFace( addPoint(S,S,S), F );
	
	// left
	F = makeNewFace();
	addVertexToFace( addPoint(0,0,S), F );
	addVertexToFace( addPoint(0,0,0), F );
	addVertexToFace( addPoint(0,S,0), F );
	addVertexToFace( addPoint(0,S,S), F );
	
	// top
	F = makeNewFace();
	addVertexToFace( addPoint(0,S,0), F );
	addVertexToFace( addPoint(S,S,0), F );
	addVertexToFace( addPoint(S,S,S), F );
	addVertexToFace( addPoint(0,S,S), F );
	
	// bottom
	F = makeNewFace();
	addVertexToFace( addPoint(0,0,0), F );
	addVertexToFace( addPoint(0,0,S), F );
	addVertexToFace( addPoint(S,0,S), F );
	addVertexToFace( addPoint(S,0,0), F );
	
	calculateNormals();
	useFaceNormals = true;
}


// ===================== poOBJShape3D ===================== //

poOBJShape3D::poOBJShape3D()
{
	
}

void	poOBJShape3D::readObjFile( char* fileName )
{
	FILE* F = fopen( fileName, "r" );
	if ( F==NULL )
	{
		printf( "ERROR: poOBJShape3D::readObjFile could not read file %s\n", fileName );
		exit(0);
		return;
	}
	
	printf("loading %s\n", fileName );
		
	char	buf[512];
	char	C;
	char	matName[128];
	int		n=0;
	
	poPoint			P, lastP;
	int				groupID = 0;
	char**			groupNameList = NULL;
	poObjMaterial*	currentMaterial = NULL;
	
	poOBJShape3D* Shape = this;
	
	while( (C = fgetc(F)) != EOF )
	{
		// capture lines
		if ( n >= 512 )
			n = 0;

		if ( C==10 || C==13 )
		{
			buf[n++] = 0;
			n = 0;
				
			//printf( "#%s#\n", buf );
			
			float x,y,z;
			
			// ignore comment lines
			if ( buf[0]=='#' )
				continue;
			
			// vertices
			if ( sscanf( buf, "v %f %f %f", &x, &y, &z ) == 3 )
			{
				Shape->addPoint( poPoint(x,y,z) );
				// printf("vertex %f %f %f\n", x,y,z );
			}
			
			// texture coords
			if ( sscanf( buf, "vt %f %f", &x, &y ) == 2 )
			{
				Shape->addTextureCoord( poPoint(x,y,0) );
				// printf("texture %f %f\n", x,y );
			}
			
			// normals
			if ( sscanf( buf, "vn %f %f %f", &x, &y, &z ) == 3 )
			{
				Shape->addNormal( poPoint(x,y,z) );
				// printf("normal %f %f %f\n", x,y,z );
			}
			
			// group id's
			if ( buf[0] == 'g' && buf[1] == ' ' )
			{
				/*groupID++;
				groupNameList = (char**) realloc( groupNameList, sizeof(char*) * groupID );
				groupNameList[ groupID-1 ] = (char*) malloc( 256 );
				sscanf( buf, "g %s", groupNameList[ groupID-1 ] );
				printf( "%d group name: %s\n", groupID-1, groupNameList[ groupID-1 ] );		// <==== print group name*/
			}
			
			// materials
			if ( sscanf( buf, "usemtl %s", matName ) == 1 )
			{
				currentMaterial = getMaterialWithName( matName );
				if ( currentMaterial == NULL )
					printf("ERROR: couldn't find material %s for OBJ file %s\n", matName, fileName );
				useFaceColors = true;
			}
			
			
			// four sided face
			if ( buf[0] == 'f' && buf[1] == ' ' )
			{
				int Face = Shape->makeNewFace();
				
				// if using materials, set face color
				if ( currentMaterial != NULL )
					faceList[ Face ].color = currentMaterial->diffuseColor;
				
				/*if ( groupID == 0 )
					Face->groupID = 0;
				else
					Face->groupID = groupID-1;*/
											
				int L = strlen(buf);
				//printf("face: ");
				for( int i=1; i<L; i++ )
				{
					if ( buf[i]==' ' )
					{
						int pInd, nInd, tInd;
						if ( sscanf( &buf[i], "%d/%d/%d", &pInd, &tInd, &nInd ) == 3 )
						{
							//printf( "%d %d %d   ", pInd, tInd, nInd );
							addVertexToFace( pInd-1, nInd-1, tInd-1, Face );
						}
						
						if ( sscanf( &buf[i], "%d//%d", &pInd, &nInd ) == 2 )
						{
							//printf( "%d %d   ", pInd, nInd );
							addVertexToFace( pInd-1, nInd-1, -1, Face );
						}
					}
				}
				//printf("\n");
			}
			
			// make new shape
			if ( buf[0] == 'g' && buf[1] == ' ' )
			{
				// group separator
			}
		
		}
		else
			buf[n++] = C;
	}
	
	/*printf("%d total points\n", Shape->pointList.size() );	
	printf("%d total normals\n", Shape->normalList.size() );	
	printf("%d total texCoords\n", Shape->textureCoordList.size() );	
	printf("%d total faces\n", Shape->faceList.size() );*/
	
	fclose( F );

	/*Shape->makeEdges();
	Shape->setEdgeSketch(0);
	Shape->makeGroups( groupNameList );*/

}


void	poOBJShape3D::readObjMtlFile( char* fileName )
{
	FILE* F = fopen( fileName, "r" );
	if ( F==NULL )
	{
		printf("ERROR poOBJShape3D::readObjMtlFile: can't open file %s\n", fileName );
		return;
	}
	else
		printf("loading OBJ material file %s\n", fileName );
	
	char buf[512];
	
	while( fgets( buf, 512, F ) )
	{	
		//printf("line: ++%s++\n", buf );
		
		char mtlNamevalue[512];
		if ( sscanf( buf, "newmtl %s", mtlNamevalue ) == 1 )
		{
			//printf("==%s===\n", mtlNamevalue);
			poObjMaterial M;
			M.name = mtlNamevalue;
			materialList.push_back( M );
		}
		
		float mtlValue0, mtlValue1, mtlValue2;
		if ( sscanf( buf, "\tKd %f %f %f", &mtlValue0, &mtlValue1, &mtlValue2  ) == 3 )
		{
			//printf("==%f %f %f===\n", mtlValue0, mtlValue1, mtlValue2 );
			materialList.back().diffuseColor.set( mtlValue0, mtlValue1, mtlValue2 );
		}
	}
	
	fclose(F);
}

poObjMaterial*	poOBJShape3D::getMaterialWithName( std::string matName )
{
	for( int i=0; i<materialList.size(); i++ )
		if ( materialList[i].name == matName )
			return &materialList[i];
	return NULL;
}


// ===================== poShape3DMaker ===================== //

/*poShape3D* poShape3DMaker::makeShape3DfromShape2D( poShape2D* S )
{
	poShape3D* S3D = new poShape3D;
	
	int faceIndex = S3D->makeNewFace();
	for( int i=0; i<S->pointList.size(); i++ )
	{
		int pointIndex = S3D->addPoint( S->pointList[i] );
		S3D->addVertexToFace( pointIndex, faceIndex );
	}
	
	S3D->fillColor = S->fillColor;
	S3D->strokeWidth = S->strokeWidth;
	S3D->strokeColor = S->strokeColor;
	S3D->enableFill = S->enableFill;
	S3D->fillDrawStyle = S->fillDrawStyle;
	S3D->strokeDrawStyle = S->strokeDrawStyle;
	S3D->position = S->position;
	S3D->rotation = S->rotation;
	S3D->rotationAxis = S->rotationAxis;
	S3D->scale = S->scale;
	
	return S3D;
}

poShape3D* poShape3DMaker::makeExtrusionFromShape2D( poShape2D* S, float Depth, bool closeShape, bool makeEndCaps )
{
	poShape3D* S3D = new poShape3D;
	
	poPoint depthVector( 0,0,Depth );
	
	int last = S->pointList.size()-1;
	if ( closeShape )
		last = S->pointList.size();
	
	// make extrusion
	for( int i=0; i<last; i++ )
	{
		int faceIndex = S3D->makeNewFace();

		int next = (i+1) % S->pointList.size();
		
		int p0_index = S3D->addPoint( S->pointList[i] );
		int p1_index = S3D->addPoint( S->pointList[next] );
		int p2_index = S3D->addPoint( S->pointList[next] + depthVector );
		int p3_index = S3D->addPoint( S->pointList[i] + depthVector );
		S3D->addVertexToFace( p0_index, faceIndex );
		S3D->addVertexToFace( p1_index, faceIndex );
		S3D->addVertexToFace( p2_index, faceIndex );
		S3D->addVertexToFace( p3_index, faceIndex );
	}
	
	// make end caps
	if ( makeEndCaps )
	{
		int faceIndex = S3D->makeNewFace();
		for( int i=0; i<S->pointList.size(); i++ )
		{
			int pointIndex = S3D->addPoint( S->pointList[i] );
			S3D->addVertexToFace( pointIndex, faceIndex );
		}
		
		faceIndex = S3D->makeNewFace();
		for( int i=0; i<S->pointList.size(); i++ )
		{
			int pointIndex = S3D->addPoint( S->pointList[i] + depthVector );
			S3D->addVertexToFace( pointIndex, faceIndex );
		}
	}
	
	S3D->calculateNormals();
	
	S3D->fillColor = S->fillColor;
	S3D->strokeWidth = S->strokeWidth;
	S3D->strokeColor = S->strokeColor;
	S3D->enableFill = S->enableFill;
	S3D->fillDrawStyle = S->fillDrawStyle;
	S3D->strokeDrawStyle = S->strokeDrawStyle;
	S3D->position = S->position;
	S3D->rotation = S->rotation;
	S3D->rotationAxis = S->rotationAxis;
	S3D->scale = S->scale;
	
	return S3D;
}

poMesh3D* poShape3DMaker::makeRevolutionFromShape2D( poShape2D* S, float radialOffset, int numSlices, float startAngle, float endAngle, bool makeEndCaps )
{
	
	poMesh3D* S3D = new poMesh3D( S->pointList.size()+1, numSlices );
	
	float dA = (endAngle - startAngle) / (float) (numSlices-1);
	printf("dA %f\n", dA );
	
	// make revolution
	for( int i=0; i<=S->pointList.size(); i++ )
	{
		poPoint shapePoint;
		if ( i == S->pointList.size() )
			shapePoint = S->pointList[0];
		else
			shapePoint = S->pointList[i];	
		
		for( int j=0; j<numSlices; j++ )
		{
			float A = dA * (float) j;
			poPoint P;
			P.y = shapePoint.y;
			P.x = (shapePoint.x+radialOffset) * cos_deg(A);
			P.z = (shapePoint.x+radialOffset) * sin_deg(A);
			int pointIndex = S3D->getMeshVertexIndex(j,i);
			S3D->pointList[pointIndex] = P;
		}
	}
	
	S3D->calculateNormals();
	
	S3D->fillColor = S->fillColor;
	S3D->strokeWidth = S->strokeWidth;
	S3D->strokeColor = S->strokeColor;
	S3D->enableFill = S->enableFill;
	S3D->fillDrawStyle = S->fillDrawStyle;
	S3D->strokeDrawStyle = S->strokeDrawStyle;
	S3D->position = S->position;
	S3D->rotation = S->rotation;
	S3D->rotationAxis = S->rotationAxis;
	S3D->scale = S->scale;
	
	return S3D;
}*/

