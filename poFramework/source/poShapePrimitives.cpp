/*
 *  poShapePrimitives.cpp
 *  poFramework2
 *
 *  Created by Joshua Fisher on 9/28/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poShapePrimitives.h"

// ====================== poRectShape ====================== //

poRectShape::poRectShape()
{
	construct( 100, 100, poColor::grey, 1, poColor::black);
}

poRectShape::poRectShape(int width, int height)
{
	construct(width, height, poColor::grey, 1, poColor::black);
}

poRectShape::poRectShape(int width, int height, const poColor& fillColor)
{
	construct(width, height, fillColor, 0, poColor::black);
}

poRectShape::poRectShape(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	construct(width, height, fillColor, strokeWidth, strokeColor);
}

poObject*	poRectShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poRectShape* obj = (poRectShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poRectShape();
	
	obj->reshape( rectWidth, rectHeight );
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void		poRectShape::init()
{
	reshape( rectWidth, rectHeight );
	poShape2D::init();
}

void	poRectShape::reshape( float W, float H )
{
	rectWidth = W;
	rectHeight = H;
	
	pointList[0].set( 0,0 );
	pointList[1].set( W,0 );
	pointList[2].set( W,H );
	pointList[3].set( 0,H );
	
	updateOrigin();
}

void poRectShape::construct(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	rectWidth = width;
	rectHeight = height;

	this->fillColor = fillColor;
	this->strokeWidth = strokeWidth;
	this->strokeColor = strokeColor;
	
	addPoint(0, 0);
	addPoint(width, 0);
	addPoint(width, height);
	addPoint(0, height);
	
	registry.addSeparator( getClassName() );
	registry.add( "rectWidth", &rectWidth );
	registry.add( "rectHeight", &rectHeight );
	
	reshape( width, height );
}


// ====================== poOvalShape ====================== //

poOvalShape::poOvalShape()
{
	construct(100, 100, poColor::grey, 1, poColor::black);
}

poOvalShape::poOvalShape(int radius)
{
	construct(radius, radius, poColor::grey, 1, poColor::black);
}

poOvalShape::poOvalShape(int radius, const poColor& fillColor)
{
	construct(radius, radius, fillColor, 0, poColor::black);
}

poOvalShape::poOvalShape(int radius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	construct(radius, radius, fillColor, strokeWidth, strokeColor);
}

poOvalShape::poOvalShape(int width, int height)
{
	construct(width, height, poColor::grey, 0, poColor::black);
}

poOvalShape::poOvalShape(int width, int height, const poColor& fillColor)
{
	construct(width, height, fillColor, 0, poColor::black);
}

poOvalShape::poOvalShape(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	construct(width, height, fillColor, strokeWidth, strokeColor);
}

poObject*	poOvalShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poOvalShape* obj = (poOvalShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poOvalShape( ovalWidth, ovalHeight );
	
	obj->reshape( ovalWidth, ovalHeight );
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void	poOvalShape::init()
{
	reshape( ovalWidth, ovalHeight );
	poShape2D::init();
}

void	poOvalShape::reshape( float W, float H )
{
	ovalWidth = W;
	ovalHeight = H;
	
	float step = 360.f / pointList.size();
	for(int i=0; i<pointList.size(); i++)
	{
		float x = dcos(step * i) * ovalWidth / 2.0;
		float y = dsin(step * i) * ovalHeight / 2.0;
		pointList[i].set( x,y );
	}
	
	updateOrigin();
}

void poOvalShape::construct(int width, int height, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	ovalWidth = width;
	ovalHeight = height;
	
	this->fillColor = fillColor;
	this->strokeWidth = strokeWidth;
	this->strokeColor = strokeColor;
	
	hOrigin = PO_ALIGN_HORI_CENTER;
	vOrigin = PO_ALIGN_VERT_CENTER;
	
	int nPoints = width;
	float step = 360.f / nPoints;
	for(int i=0; i<nPoints; i++)
	{
		float x = dcos(step * i) * width / 2.0;
		float y = dsin(step * i) * height / 2.0;
		addPoint(x, y);
	}
	
	registry.addSeparator( getClassName() );
	registry.add( "ovalWidth", &ovalWidth );
	registry.add( "ovalHeight", &ovalHeight );
	
	reshape( width, height );
}

// ====================== poRoundRectShape ====================== //

poRoundRectShape::poRoundRectShape()
{
	construct( 100,100,10, poColor::grey, 1, poColor::black);
}

poRoundRectShape::poRoundRectShape(int width, int height, float cornerRadius)
{
	construct(width, height, cornerRadius, poColor::black, 0, poColor::black);
}

poRoundRectShape::poRoundRectShape(int width, int height, float cornerRadius, const poColor& fillColor)
{
	construct(width, height, cornerRadius, fillColor, 0, poColor::black);
}

poRoundRectShape::poRoundRectShape(int width, int height, float cornerRadius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{
	construct(width, height, cornerRadius, fillColor, strokeWidth, strokeColor);
}

void poRoundRectShape::construct(int width, int height, float cornerRadius, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{	
	this->fillColor = fillColor;
	this->strokeWidth = strokeWidth;
	this->strokeColor = strokeColor;
	
	// 15 points per corner
	for(int i=0; i<60; i++)
		addPoint(0, 0);
	
	reshape( width, height, cornerRadius );
	
	registry.addSeparator( getClassName() );
	registry.add( "rectWidth", &this->rectWidth );
	registry.add( "rectHeight", &this->rectHeight );
	registry.add( "cornerRadius", &this->cornerRadius );
}

poObject* poRoundRectShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poRoundRectShape* obj = (poRoundRectShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poRoundRectShape( rectWidth, rectHeight, cornerRadius );
	
	obj->reshape( rectWidth, rectHeight, cornerRadius );
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void poRoundRectShape::init()
{
	reshape( rectWidth, rectHeight, cornerRadius );	
	poShape2D::init();
}

void poRoundRectShape::reshape( float W, float H, float rad )
{
	rectWidth = W;
	rectHeight = H;
	cornerRadius = rad;
	
	float step = 90.f / 15;
	
	for(int i=0; i<15; i++)
	{
		float x = dcos(step * i - 180) * cornerRadius + cornerRadius;
		float y = dsin(step * i - 180) * cornerRadius + cornerRadius;
		pointList[i].set( x,y );
	}
	
	for(int i=0; i<15; i++)
	{
		float x = dcos(step * i - 90) * cornerRadius - cornerRadius + rectWidth;
		float y = dsin(step * i - 90) * cornerRadius + cornerRadius;
		pointList[15+i].set( x,y );
	}
	
	for(int i=0; i<15; i++)
	{
		float x = dcos(step * i) * cornerRadius - cornerRadius + rectWidth;
		float y = dsin(step * i) * cornerRadius - cornerRadius + rectHeight;
		pointList[30+i].set( x,y );
	}
	
	for(int i=0; i<15; i++)
	{
		float x = dcos(step * i + 90) * cornerRadius + cornerRadius;
		float y = dsin(step * i + 90) * cornerRadius - cornerRadius + rectHeight;
		pointList[45+i].set( x,y );
	}
	
	updateOrigin();
}

void	poRoundRectShape::reshape( float W, float H )
{
	reshape( W, H, cornerRadius );
}


// ====================== poLineShape ====================== //

poLineShape::poLineShape()
{
	construct( poPoint(0,0,0), poPoint(100,0,0) );
}

poLineShape::poLineShape( poPoint A, poPoint B )
{
	construct( A, B );
}

void	poLineShape::construct( poPoint A, poPoint B )
{
	enableFill = false;
	strokeWidth = 1;
	strokeDrawStyle = GL_LINE_STRIP;
	addPoint( A );
	addPoint( B );
	
	registry.addSeparator( getClassName() );
	registry.add( "point A", &A );
	registry.add( "point B", &B );
}

void	poLineShape::reshape( poPoint A, poPoint B )
{
	this->A = A;
	this->B = B;

	pointList[0] = A;
	pointList[1] = B;
}

poObject* poLineShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poLineShape* obj = (poLineShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poLineShape();
	
	obj->reshape( A, B );
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void poLineShape::init()
{
	reshape( A, B );
	poShape2D::init();
}



// ====================== poBezierShape ====================== //

poBezierShape::poBezierShape()
{
	construct();
}

void	poBezierShape::construct()
{
	enableFill = false;
	strokeWidth = 1;
	strokeDrawStyle = GL_LINE_STRIP;
	numPointsPerLine = 10;
	bezierGenerated = false;
	
	registry.addSeparator( getClassName() );
}


poObject* poBezierShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poBezierShape* obj = (poBezierShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poBezierShape();
	
	obj->numPointsPerLine = numPointsPerLine;
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void poBezierShape::init()
{
	poShape2D::init();
}

void poBezierShape::addPoint(float x, float y)
{ 
	bezierGenerated = false; 
	bezierPointList.push_back( poPoint(x,y) ); 
}

void poBezierShape::generateBezierPoints()
{	
	pointList.clear();
	int L = bezierPointList.size()/4;
	for( int i=0; i< L; i++ )
	{
		for( int n=0; n< numPointsPerLine; n++ )
		{
			poPoint A = bezierPointList[ i*4 ];
			poPoint B = bezierPointList[ i*4+1 ];
			poPoint C = bezierPointList[ i*4+2 ];
			poPoint D = bezierPointList[ i*4+3 ];
			float p = (float) n / (float) (numPointsPerLine-1);
			float b = 1.0 - p;
			poPoint BP = A*b*b*b + 3*B*b*b*p + 3*C*b*p*p + D*p*p*p;
			pointList.push_back( BP );
		}
	}
	bezierGenerated = true;
}

void poBezierShape::update()
{
	if( !bezierGenerated )
		generateBezierPoints();
}


// ====================== poAnimatedStrokeShape ====================== //

poAnimatedStrokeShape::poAnimatedStrokeShape()
{
	construct();
}

void poAnimatedStrokeShape::construct()
{
	strokeDrawStyle = GL_LINE_STRIP;
	strokePercent = 0.0;
	totalLength = 0.0;
}

void poAnimatedStrokeShape::addShape( poShape2D* shape )
{
	this->strokeDrawStyle = shape->strokeDrawStyle;
	this->pointList = shape->pointList;
}

void poAnimatedStrokeShape::draw()
{
#ifdef WIN32
	glDisable( GL_TEXTURE_2D );
#else
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
#endif
	if( pointList.size() < 1 ) return;
	
	int finish = pointList.size()-1;
	if( strokeDrawStyle == GL_LINE_LOOP ) finish += 1;
	if( totalLength == 0 ){
		for( int i=0; i< finish; i++ )
			totalLength += (pointList[i % pointList.size()] - pointList[(i+1) % pointList.size()]).length();
	}
	
	glLineWidth(strokeWidth);
	glColor4f(strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A*masterAlphaControl);
	
	float p = 0.0;
	for( int i=0; i< finish; i++ )
	{
		poPoint thisPoint = pointList[i % pointList.size()];
		poPoint nextPoint = pointList[(i+1) % pointList.size()];
		float L = (thisPoint - nextPoint).length() / totalLength;
		p += L;
		
		glBegin( GL_LINE_STRIP );
		glVertex3fv((float*)&thisPoint);
		if( p <= strokePercent ){
			glVertex3fv((float*)&nextPoint);
			glEnd();
		}else{
			float F = ( strokePercent - (p - L) )*totalLength;
			poPoint E = thisPoint + (nextPoint - thisPoint).normalize()*F;
			glVertex3fv((float*)&E);
			glEnd();
			break;
		}
	}
}


// ====================== poImageShape ====================== //

poImageShape::poImageShape()
{
}

poImageShape::poImageShape( char* imageFileName )
{
	poTexture* T = poTextureStore::get( imageFileName );
	if ( T != NULL )
		construct( T, 0, 0, poColor::white, 0, poColor::black );
}

poImageShape::poImageShape( poTexture* tex, int maxWidth, int maxHeight )
{
	construct( tex, maxWidth, maxHeight, poColor::white, 0, poColor::black );
}

poImageShape::poImageShape( poTexture* tex, int maxWidth, int maxHeight, const poColor& fillColor )
{
	construct( tex, maxWidth, maxHeight, fillColor, 0, poColor::black );
}

void poImageShape::construct( poTexture* tex, int maxWidth, int maxHeight, const poColor& fillColor, int strokeWidth, const poColor& strokeColor)
{	
	this->fillColor = fillColor;
	this->strokeWidth = strokeWidth;
	this->strokeColor = strokeColor;

	this->addModifier( tex );
	tex->initWithObject( this );
	
	rectWidth = tex->width;
	rectHeight = tex->height;
	
	if( maxWidth != 0 && maxHeight != 0 )
	{
		float tmpHeight = maxWidth * rectHeight / rectWidth;
		if( tmpHeight <= maxHeight )
		{
			reshape( maxWidth, tmpHeight );
		}
		else
		{
			float tmpWidth = maxHeight * rectWidth / rectHeight;
			reshape( tmpWidth, maxHeight );
		}
	}
	
}

void	poImageShape::reshape( float W, float H )
{
	rectWidth = W;
	rectHeight = H;
	
	pointList[0].set( 0,0 );
	pointList[1].set( W,0 );
	pointList[2].set( W,H );
	pointList[3].set( 0,H );
	
	updateOrigin();
}

poObject* poImageShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poImageShape* obj = (poImageShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poImageShape();
	
	obj->reshape( rectWidth, rectHeight );
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void poImageShape::init()
{
	reshape( rectWidth, rectHeight );
	poShape2D::init();
}




// ====================== poArcShape ====================== //

/*poArcShape::poArcShape()
{
	construct( 50, 100, 0, 90 );
}

poArcShape::poArcShape( float innerRadius, float outerRadius, float startAngle, float endAngle )
{
	construct( innerRadius, outerRadius, startAngle, endAngle );
}

void poArcShape::construct( float innerRadius, float outerRadius, float startAngle, float endAngle )
{
	int	numSteps = (startAngle - endAngle) / 2.0;
	float step = (startAngle - endAngle) / numSteps;
	
	for( int i=0; i<=numSteps; i++ )
	{
		float A = i*step;
		addPoint( poPoint( innerRadius*dcos(A), innerRadius*dsin(A) ) );
		addPoint( poPoint( outerRadius*dcos(A), outerRadius*dsin(A) ) );
	}
	
	fillDrawStyle = GL_TRIANGLE_STRIP;
}


poObject* poArcShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poArcShape* obj = (poArcShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poArcShape();
	
	obj->reshape( rectWidth, rectHeight );
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}

void poArcShape::init()
{
}*/

