/*
 *  poVisionImageBuffer.cpp
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poVisionImageBuffer.h"

bool	poVisionImageBuffer::FLIP_VIDEO_VERT = true; //false;
bool	poVisionImageBuffer::FLIP_VIDEO_HORZ = false;


poVisionImageBuffer::poVisionImageBuffer( int W, int H )
{
	width = W;
	height = H;
	
	image = (unsigned char*) malloc( width*height );
	memset( image, 0, width*height );
}


poVisionImageBuffer::poVisionImageBuffer( int W, int H, const char* rawImageFileName )
{
	width = W;
	height = H;
	
	image = (unsigned char*) malloc( width*height );
	FILE* F = fopen( rawImageFileName, "r" );
	if ( F==NULL )
	{
		printf("ERROR: poVisionImageBuffer could not read file %s\n", rawImageFileName );
		exit(0);
	}
	fread( image, width*height, 1, F );
	fclose(F);
	

	/*poImage* img = new poImage( rawImageFileName );
	//printf( rawImageFileName );
	for( int i=0; i< width; i++ ){
		for( int j=0; j< height; j++ ){
			image[ j*width + i ] = img->getSubPixel( i, height- j-1, 0 );
		}
	}*/
	
}


poVisionImageBuffer::~poVisionImageBuffer()
{
	if ( image )
		free( image	 );
}


void	poVisionImageBuffer::copy( unsigned char* newImage )
{
	memcpy( image, newImage, width*height );
}


void	poVisionImageBuffer::copy( poVisionImageBuffer* newImage )
{
	memcpy( image, newImage->image, width*height );
}


void	poVisionImageBuffer::draw( int x, int y, float scale )
{
	
	float vDir = 1;
	float vOff = 0;
	if( FLIP_VIDEO_VERT ) { vDir = -1; vOff = 1; }
	float hDir = 1;
	float hOff = 0;
	if( FLIP_VIDEO_HORZ ) { hDir = -1; hOff = 1; }

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4 );
	glPixelStorei(GL_UNPACK_ROW_BYTES_APPLE, 0 );
	
	// glRasterPos2i( x+(width*hOff*scale),y+(height*vOff*scale) );
    glRasterPos2i( x,y );
	glPixelZoom( scale*hDir,scale*vDir );
	
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	glDrawPixels( width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image );
}


void	poVisionImageBuffer::drawAsOverlay( int x, int y, poColor overlayColor )
{
	//overlayColor.apply();
	glColor3f( 1,0,0 );
	
	glPushMatrix();
	
	// put video in place
	float vDir = 1;
	float vOff = 0;
	if( poVisionImageBuffer::FLIP_VIDEO_VERT ) { vDir = -1; vOff = 1; }
	float hDir = 1;
	float hOff = 0;
	if( poVisionImageBuffer::FLIP_VIDEO_HORZ ) { hDir = -1; hOff = 1; }

	glScalef( hDir,vDir,1 );
	glTranslatef( x+width*hOff, y+height*vOff, 1 );
	
	// draw overlay video
	for( int i=0; i<width; i++ )
	{
		for( int j=0; j<height; j++ )
		{
			int V = image[(j*width+i)];
			if ( V != 0 )
				glRectf( i, j, i+1, j+1 );
		}
	}
	glPopMatrix();
	
}

void	poVisionImageBuffer::drawBlacks( int x, int y, poColor overlayColor )
{
	//overlayColor.apply();
	glColor3f( 1,0,0 );
	
	glPushMatrix();
	
	// put video in place
	float vDir = 1;
	float vOff = 0;
	if( poVisionImageBuffer::FLIP_VIDEO_VERT ) { vDir = -1; vOff = 1; }
	float hDir = 1;
	float hOff = 0;
	if( poVisionImageBuffer::FLIP_VIDEO_HORZ ) { hDir = -1; hOff = 1; }

	glScalef( hDir,vDir,1 );
	glTranslatef( x+width*hOff, y+height*vOff, 1 );
	
	// draw overlay video
	for( int i=0; i<width; i++ )
	{
		for( int j=0; j<height; j++ )
		{
			int V = image[(j*width+i)];
			if ( V == 0 )
				glRectf( i, j, i+1, j+1 );
		}
	}
	glPopMatrix();
	
}


int		poVisionImageBuffer::calculateChecksum()
{
	int checksum = 0;
	for( int i=0; i<width*height; i++ )
		checksum += image[i];
	return checksum;
}


void	poVisionImageBuffer::shift( int dX, int dY )
{
	poVisionImageBuffer* tmpBuffer = new poVisionImageBuffer( width, height );
	tmpBuffer->copy( this );
	
	// shift image by dX, dY (dY is reversed if image is not flipped)
	for( int i=0; i<width; i++ )
	{
		for( int j=0; j<height; j++ )
		{			
			int i2 = i-dX;
			int j2 = j+dY;
			if ( i2 < 0 || i2 >= width || j2<0 || j2>=height )
				setPixel( i,j,0 );
			else
				setPixel( i,j, tmpBuffer->getPixel(i2,j2) );
		}
	}
	
	delete tmpBuffer;
}


void	poVisionImageBuffer::setAllPixels( int v )
{
	memset( image, v, width*height );
}


void	minIntSwap( int& A, int &B )
{
	if ( A > B )
	{
		int tmp = B;
		B = A;
		A = tmp;
	}
}

void	poVisionImageBuffer::fillRect( int x0, int y0, int x1, int y1, int v )
{
	y0 = height - y0 - 1;
	y1 = height - y1 - 1;
	
	minIntSwap( x0,x1 );
	minIntSwap( y0,y1 );
	
	for( int i=x0; i<=x1; i++ )
	{
		for( int j=y0; j<=y1; j++ )
		{
			setPixel( i,j, v );
		}
	}
}


void	poVisionImageBuffer::drawLine( int x0, int y0, int x1, int y1, int v )
{
	y0 = height - y0 - 1;
	y1 = height - y1 - 1;
	
	minIntSwap( x0,x1 );
	minIntSwap( y0,y1 );

	float dX = (x1-x0);
	float dY = (y1-y0);
	float dist = sqrt( dX*dX + dY*dY );
	
	float dT = 1.0 / dist / 2;
	for( float t=0; t<=1.001; t+=dT  )
	{
		int pX = x0 + dX*t;
		int pY = y1 - dY*t;
		setPixel( pX,pY,v );
	}
}


bool	poVisionImageBuffer::write( const char* fileName )
{
	FILE *F = fopen(fileName, "w");
	if ( F==NULL )
	{
		printf("ERROR: couldn't write image buffer %s\n", fileName );
		return false;
	}
	fwrite( (void*) image, sizeof(char), width*height, F );
	fclose( F );
	return true;
}

bool	poVisionImageBuffer::read( const char* fileName )
{
	FILE *F = fopen(fileName,"r");
	if ( F==NULL )
	{
		printf("ERROR: couldn't read image buffer %s\n", fileName );
		return false;
	}
	fread( (void*) image, sizeof(char), width*height, F );
	fclose( F );
	return true;
}