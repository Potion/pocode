/*
 *  poLayout.cpp
 *  poFramework_static_lib
 *
 *  Created by Young Cho on 7/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poLayout.h"
#include "poShape2D.h"

// ============================== poTextOnPath ================================= //

poTextOnPath::poTextOnPath()
{
	construct();
}

poTextOnPath::poTextOnPath(int width, int height) : poTextBox( width, height )
{
	construct();
}

poTextOnPath::poTextOnPath(const char* text, int width, int height) : poTextBox( text, width, height )
{
	construct();
}

poTextOnPath::poTextOnPath(const char* text, int width, int height, const poColor& fontColor ) : poTextBox( text, width, height, fontColor )
{
	construct();
}

poTextOnPath::poTextOnPath(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor )
: poTextBox( text, width, height, fontColor, fillColor, strokeWidth, strokeColor )
{
	construct();
}

poTextOnPath::~poTextOnPath()
{

}

void poTextOnPath::construct()
{
	pathLength = 0;
	loopText = false;
}

void poTextOnPath::addPoint( float x, float y )
{
	pointList.push_back( poPoint( x, y ) );
}

void poTextOnPath::addShape( poShape2D* shape )
{
	pointList = shape->pointList;
	if( shape->strokeDrawStyle == GL_LINE_LOOP )
		pointList.push_back( shape->pointList[0] );
}

void poTextOnPath::calculatePath()
{
	if( pointList.size() < 1 ) return;
	
	pathLength = 0;
	for( int i=0; i< pointList.size()-1; i++ )
	{
		float L = ( pointList[i+1] - pointList[i] ).length();
		pointList[i].w = pathLength;
		pathLength += L;
	}
	pointList.back().w = pathLength;
}

void poTextOnPath::updateLayout()
{
	calculatePath();
	
	pathLetterList.clear();
	
	for( int i=0; i< letterList.size(); i++ )
	{
		poTBLetter C = letterList[i];
		
		// find the closest point on path for letter
		float letterX = letterList[i].position.x + offset.x;
		if( loopText )
		{
			if( letterX > pathLength )
				letterX = letterX - pathLength*(int)(letterX/pathLength);
			else if( letterX < 0 )
				letterX = pathLength*( 1+ (int)(abs(letterX/pathLength)) ) + letterX;
		}
		
		int currPoint = 0;
		while( letterX >= pointList[currPoint].w ){
			currPoint++;
			if( currPoint > pointList.size()-1 )
				break;
		}
		
		if( currPoint > 0 ) currPoint--;
		if( currPoint >= pointList.size()-1 )
			continue;
		
		// calculate the exact point and angle on path
		float d = letterX - pointList[currPoint].w;
		if( d==0 ) d = 0.0001;
		if( d < 0 ) continue;
		
		C.offset.y = C.position.y;
		poPoint P = pointList[currPoint] + (pointList[currPoint+1]-pointList[currPoint]).normalize()*d;
		
		if( offset.y == 0 )
			C.position =  P;
		else
		{
			// find right angle vector
			poPoint R = pointList[currPoint+1]-P;
			R.set( -R.y, R.x );
			C.position = P + R.normalize()*offset.y;
		}
		
		// calculate angle
		poPoint V = P - pointList[currPoint];
		C.position.w = atan2( V.y, V.x )* 180.0 / 3.14159;
		pathLetterList.push_back( C );
	}
}


void poTextOnPath::draw()
{
	glAlphaFunc(GL_GREATER,0.01);
	glEnable(GL_ALPHA_TEST);
	
	for( int i=0; i< pathLetterList.size(); i++ )
	{
		glPushMatrix();
		poTBLetter& letter = pathLetterList[i];
		glColor4f(letter.color.R, letter.color.G, letter.color.B, letter.color.A*masterAlphaControl);
		glTranslatef( letter.position.x, letter.position.y, letter.position.z );
		glRotatef( letter.position.w, 0,0,1 );
		glTranslatef( letter.offset.x, letter.offset.y, 0 );
		letter.font->drawChar(letter.c, 0, 0, letter.scale );
		glPopMatrix();
	}
	
	glDisable(GL_ALPHA_TEST);
}



// ============================== poTimeLine ================================= //

poTimeLine::poTimeLine()
{
	timer = 0;
	currFrame = -1;
	motionPercent = 0.0;
	
	this->emphasis = 0;
	this->duration = 0;
	this->animationType = PO_LINEAR;
}

void poTimeLine::update()
{
	if( timer > duration ) return;
	
	float timeParam =  timer / (float)duration;
	if( timeParam > 1.0 ) timeParam = 1.0;
	
	if( animationType == PO_JUMP )
		motionPercent = 1.0;
	if( animationType == PO_LINEAR )
		motionPercent = timeParam;
	if( animationType == PO_LINEAR )
		motionPercent = timeParam;
	if( animationType == PO_EASE_IN )
		motionPercent = pow( timeParam, emphasis );
	if( animationType == PO_EASE_OUT )
		motionPercent = pow((float)timeParam, (float)1.0/emphasis);
	if( animationType == PO_EASE_IN_OUT )
		motionPercent = 0.5*cos(3.14159*(timeParam-1.0)) + 0.5;
	if( animationType == PO_JITTER )
	{
		motionPercent = timeParam + poRand()*emphasis;
		if( timeParam >= 0.999 )
			motionPercent = 1.0;
	}
	
	if( timeParam <= 0.0 ) motionPercent = 0.0;
	if( timeParam >= 1.0 ) motionPercent = 1.0;
	
	position = thisStart.position*(1.0-motionPercent) + thisEnd.position*motionPercent;
	alpha = thisStart.alpha*(1.0-motionPercent) + thisEnd.alpha*motionPercent;
	scale = thisStart.scale*(1.0-motionPercent) + thisEnd.scale*motionPercent;
	rotation = thisStart.rotation*(1.0-motionPercent) + thisEnd.rotation*motionPercent;
	
	for( int i=0; i< this->numChildren(); i++ )
	{
		poObject* obj = this->getChild( i );
		obj->position = childrenStartList[i].position*(1.0-motionPercent) + childrenEndList[i].position*motionPercent;
		obj->alpha = childrenStartList[i].alpha*(1.0-motionPercent) + childrenEndList[i].alpha*motionPercent;
		obj->scale = childrenStartList[i].scale*(1.0-motionPercent) + childrenEndList[i].scale*motionPercent;
		obj->rotation = childrenStartList[i].rotation*(1.0-motionPercent) + childrenEndList[i].rotation*motionPercent;
	}
	
	timer++;
}

void poTimeLine::captureFrame()
{
	// capture this object's property
	poObjectProperties P( this->position, this->alpha, this->scale, this->rotation, this->rotationAxis, this->visible );
	thisPropertyList.push_back( P );
	
	// capture children's property
	poObjectPropertyVector objProperty;
	for( int i=0; i< this->numChildren(); i++ )
	{
		poObject* obj = this->getChild( i );
		poObjectProperties CP = obj->getProperties();
		objProperty.push_back( CP );
		childrenStartList.push_back( CP );
		childrenEndList.push_back( CP );
	}
	childrenPropertyList.push_back( objProperty );
}

void poTimeLine::setFrame( int frame, poAnimationStyle animationType, float emphasis, int duration )
{
	if( frame == currFrame || frame > childrenPropertyList.size()-1 )
		return;
	
	poObjectProperties P = this->getProperties();
	thisStart = P;
	thisEnd = thisPropertyList[frame];
	
	childrenStartList.clear();
	childrenEndList.clear();
	
	for( int i=0; i< this->numChildren(); i++ )
	{
		poObject* obj = this->getChild( i );
		
		// set beginning point
		poObjectProperties P = obj->getProperties();
		childrenStartList.push_back( P );
		
		// set destination
		childrenEndList.push_back( childrenPropertyList[frame][i] );
	}
	
	currFrame = frame;
	this->emphasis = emphasis;
	this->duration = duration;
	this->animationType = animationType;
	timer = 0;
}




// ============================== poStampPath ================================= //

poStampPath::poStampPath()
{
	stampObj = NULL;
}

void poStampPath::addShape(poObject* stampObj )
{
	this->stampObj = stampObj;
}

void poStampPath::setup(poObject* obj)
{
	if( !stampObj )
		return;
	
	poShape2D* shape = dynamic_cast<poShape2D*>(obj);
	if( !shape )
		return;
	
	for( int i=0; i< shape->pointList.size(); i++ )
	{
		stampObj->position = shape->pointList[i];
		
		// calculate rotation
		float originalRotation = stampObj->rotation;
		float newRotation = 0;
		if( i==0 ){
			poPoint V1 = shape->pointList[i+1] - shape->pointList[i];
			newRotation = atan2( V1.y, V1.x ) / 3.14159*180;
		}else if( i== shape->pointList.size()-1 ){
			poPoint V1 = shape->pointList[i] - shape->pointList[i-1];
			newRotation = atan2( V1.y, V1.x ) / 3.14159*180;
		}else
		{
			poPoint V1 = shape->pointList[i+1] - shape->pointList[i];
			poPoint V2 = shape->pointList[i] - shape->pointList[i-1];
			newRotation = ( atan2( V1.y, V1.x ) + atan2( V2.y, V2.x ) )*0.5 / 3.14159*180;
		}
		stampObj->rotation += newRotation;
		stampObj->drawTree();
		stampObj->rotation = originalRotation;
	}
}

void poStampPath::setdown(poObject* obj)
{
}

// ============================== poAnimationPath ================================= //

poAnimationPath::poAnimationPath()
{
	totalPathLength = 0;
	motionPercent = 0;
	loop = false;
	
	doDrawing = false;
	strokeWidth = 1;
	strokeColor.set( 1,1,1 );
}

void poAnimationPath::addPoint(float x, float y)
{
	pointList.push_back( poPoint(x,y) );
}

void poAnimationPath::addShape(poShape2D* shape)
{
	if( shape->pointList.size() < 1 ) 
		return;
	
	totalPathLength = 0;
	pointList.clear();
	pointList = shape->pointList;
	if( shape->strokeDrawStyle == GL_LINE_LOOP )
		pointList.push_back( shape->pointList[0] );
}

void poAnimationPath::setup(poObject* obj)
{
	if( !obj )
		return;
	
	if( doDrawing )
		drawPath();
	
	updatePoint(obj);
	//obj->position = objPosition;
	glPushMatrix();
	glTranslatef( objPosition.x, objPosition.y, objPosition.z );
	
}

void poAnimationPath::setdown(poObject* obj)
{
	glPopMatrix();
}

void poAnimationPath::drawPath()
{
	glLineWidth( strokeWidth );
	glColor4f( strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i< pointList.size(); i++ )
		glVertex3fv( (float*)(&pointList[i]) );
	glEnd();
}

void poAnimationPath::updatePath()
{
	totalPathLength = 0;
	for( int i=0; i< pointList.size()-1; i++ )
	{
		float L = ( pointList[i+1] - pointList[i] ).length();
		pointList[i].w = totalPathLength;
		totalPathLength += L;
	}
	pointList.back().w = totalPathLength;
}

void poAnimationPath::updatePoint(poObject* obj)
{
	if( totalPathLength == 0 )
		updatePath();
	
	int currPoint = 0;
	if( motionPercent > 1.0 ) motionPercent = motionPercent - 1.0;
	if( motionPercent < 0.0 ) motionPercent = 1.0 + motionPercent;
	float L = motionPercent*totalPathLength;
	if( loop ){
		if( L > totalPathLength )
			L = L - totalPathLength*(int)(L/totalPathLength);
		else if( L < 0 )
			L = totalPathLength*( 1+ (int)(abs(L/totalPathLength)) ) + L;
	}
	while( L > pointList[currPoint].w ){
		currPoint++;
		if( currPoint > pointList.size()-1 )
			break;
	}
	
	if( currPoint > 0 ) currPoint--;
	if( currPoint > pointList.size()-1 ) return;

	float d = L - pointList[currPoint].w;
	objPosition = pointList[currPoint] + (pointList[currPoint+1]-pointList[currPoint]).normalize()*d;

}


// ============================== poLayout ================================= //

poLayout::poLayout()
{ 
	layoutFinished = false;
	init();
}

void poLayout::init()
{
	hAlign = PO_ALIGN_LEFT;
	vAlign = PO_ALIGN_TOP;
}

void poLayout::setAlign( poHoriOrientation hAlign, poVertOrientation vAlign)
{
	this->hAlign = hAlign;
	this->vAlign = vAlign;
}


// ============================== poGridLayout ================================= //

poGridLayout::poGridLayout( int numCols, int numRows, float spacing )
{
	construct( numCols, numRows, spacing, spacing );
}

poGridLayout::poGridLayout( int numCols, int numRows, float horiSpacing, float vertSpacing )
{
	construct( numCols, numRows, horiSpacing, vertSpacing );
}

void poGridLayout::construct( int numCols, int numRows, float horiSpacing, float vertSpacing )
{
	this->numCols = numCols;
	this->numRows = numRows;
	this->horiSpacing = horiSpacing;
	this->vertSpacing = vertSpacing;
	
	gridW = 0;
	gridH = 0;
	colMaxWidth = new float[numCols];
	rowMaxHeight = new float[numRows];
	
	doDrawing = false;
	strokeWidth = 2;
	strokeColor.set( 1,1,1 );
}

void poGridLayout::setup( poObject* obj )
{
	if( !layoutFinished ) doLayout( obj );
	if( doDrawing ) drawLayout();
}

void poGridLayout::setdown( poObject* obj )
{
}

void poGridLayout::doLayout( poObject* obj )
{
	if( numCols < 1 || numRows < 1 || obj->numChildren() > numCols*numRows ){
		printf( "ERROR : poGridLayout: grid is too small.\n" );
		return;
	}
	
	// check maximum width in each col
	for( int i=0; i< numCols; i++ )
	{
		float maxWidth = 0;
		for( int j=0; j< numRows; j++ )
		{
			if( j*numCols + i > obj->numChildren()-1 )
				break;
			
			float W = 0;
			poObject* object = obj->getChild( i+j*numCols );
			poTextBox* textBox = dynamic_cast<poTextBox*>(object);
			if( textBox ) W = textBox->getActualBounds().right - textBox->getActualBounds().left;
			else W = object->getWidth();
			
			if( W > maxWidth ) maxWidth = W;
		}
		colMaxWidth[i] = maxWidth;
	}

	// check maximum height in each row
	for( int j=0; j< numRows; j++ )
	{
		float maxHeight = 0;
		for( int i=0; i< numCols; i++ )
		{
			if( j*numCols + i > obj->numChildren()-1 )
				break;
			
			poObject* object = obj->getChild( i+j*numCols );
			poTextBox* textBox = dynamic_cast<poTextBox*>(object);
			float H = 0;
			if( dynamic_cast<poTextBox*>(object) ) H = textBox->getActualBounds().top - textBox->getActualBounds().bottom;
			else H = object->getHeight();
			
			if( H > maxHeight ) maxHeight = H;
		}
		rowMaxHeight[j] = maxHeight;
	}

	// place objects on grid
	gridH = 0;
	for( int j=0; j< numRows; j++ )
		gridH += rowMaxHeight[j]+vertSpacing;
	gridH -= vertSpacing;
	
	float X = 0;
	float Y = gridH;
	for( int i=0; i< numCols; i++ )
	{
		for( int j=0; j< numRows; j++ )
		{
			if( j*numCols + i > obj->numChildren()-1 )
				break;
			
			poObject* object = obj->getChild( i+j*numCols );
			poTextBox* textBox = dynamic_cast<poTextBox*>(object);
			if( textBox )
			{
				textBox->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
				if( textBox->staticTexture ) textBox->makeStaticTexture();
				else textBox->doLayout();
			}else
				object->setOrigin( hAlign, vAlign );
			
			if( j > 0 ) Y -= rowMaxHeight[j] + vertSpacing;
			else Y -= rowMaxHeight[j];
			
			// modify position according to alignment
			float actualX = 0;
			float actualY = 0;
			
			if( hAlign == PO_ALIGN_LEFT ) actualX = X;
			else if( hAlign == PO_ALIGN_RIGHT ) actualX = X + colMaxWidth[i];
			else if( hAlign == PO_ALIGN_HORI_CENTER ) actualX = X + colMaxWidth[i]/2.0;
			
			if( vAlign == PO_ALIGN_TOP ) actualY = Y + rowMaxHeight[j];
			else if( vAlign == PO_ALIGN_BOTTOM ) actualY = Y;
			else if( vAlign == PO_ALIGN_VERT_CENTER ) actualY = Y + rowMaxHeight[j]/2.0;
			
			// manual setOrigin for textBox
			if( textBox )
			{
				if( hAlign == PO_ALIGN_LEFT ) actualX -= 0;
				else if( hAlign == PO_ALIGN_RIGHT ) actualX -= (textBox->getActualBounds().right - textBox->getActualBounds().left);
				else if( hAlign == PO_ALIGN_HORI_CENTER ) actualX -= (textBox->getActualBounds().right - textBox->getActualBounds().left)/2.0;
				
				if( vAlign == PO_ALIGN_TOP ) actualY -= 0;
				else if( vAlign == PO_ALIGN_BOTTOM ) actualY += (textBox->getActualBounds().top - textBox->getActualBounds().bottom);
				else if( vAlign == PO_ALIGN_VERT_CENTER ) actualY += (textBox->getActualBounds().top - textBox->getActualBounds().bottom)/2.0;
			}
			
			object->position.set( actualX, actualY );
		}
		X += colMaxWidth[i] + horiSpacing;
		Y = gridH;
	}
	gridW = X - horiSpacing;

	layoutFinished = true;
}


void poGridLayout::drawLayout()
{
	glLineWidth( strokeWidth );
	glColor4f( strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A );
	glBegin( GL_LINES );
	float X = 0;
	for( int i=0; i< numCols; i++ )
	{
		glVertex2f( X, 0 );
		glVertex2f( X, gridH );
		X += colMaxWidth[i];
		
		glVertex2f( X, 0 );
		glVertex2f( X, gridH );
		X += horiSpacing;
	}
	float Y = gridH;
	for( int j=0; j< numRows; j++ )
	{
		glVertex2f( 0, Y );
		glVertex2f( gridW, Y );
		Y -= rowMaxHeight[j];
		
		glVertex2f( 0, Y );
		glVertex2f( gridW, Y );
		Y -= vertSpacing;
	}
	glEnd();
}


// ============================== poHorizontalLayout ================================= //

poHorizontalLayout::poHorizontalLayout() : poGridLayout()
{
	horiSpacing = 0;
}

poHorizontalLayout::poHorizontalLayout( float spacing )
{
	horiSpacing = spacing;
}

void poHorizontalLayout::setup(poObject* obj)
{
	if( !layoutFinished )
	{
		construct( obj->numChildren(), 1, horiSpacing, horiSpacing );
		doLayout(obj);
	}
}

void poHorizontalLayout::setdown(poObject* obj)
{
}


// ============================== poVerticalLayout ================================= //

poVerticalLayout::poVerticalLayout()
{
	vertSpacing = 0;
}

poVerticalLayout::poVerticalLayout( float spacing)
{
	vertSpacing = spacing;
}

void poVerticalLayout::setup(poObject* obj)
{
	if( !layoutFinished )
	{
		construct( 1, obj->numChildren(), vertSpacing, vertSpacing );
		doLayout(obj);
	}
}

void poVerticalLayout::setdown(poObject* obj)
{
}