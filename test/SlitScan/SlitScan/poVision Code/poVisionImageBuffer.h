

/*
 *  poVisionImageBuffer.h
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PO_VISION_IMAGE_BUFFER_H
#define PO_VISION_IMAGE_BUFFER_H


#include "poColor.h"
#include <math.h>
//#include "poCommon.h"
#include "poImage.h"
//#include <OpenGL/gl.h>

class poVisionImageBuffer
{
public:
	poVisionImageBuffer( int W, int H );
	poVisionImageBuffer( int W, int H, const char* rawImageFileName );
	~poVisionImageBuffer();
	
	void					copy( unsigned char* newImage );
	void					copy( poVisionImageBuffer* newImage );
	
	void					draw( int x, int y, float scale=1.0 );
	void					drawAsOverlay( int x, int y, poColor overlayColor );
	void					drawBlacks( int x, int y, poColor overlayColor );

	inline unsigned char	getPixel( int i, int j ) { return image[j*width+i]; };
	inline void				setPixel( int i, int j, int v ) { image[j*width+i] = v; };
	inline unsigned char	getPixel( int n ) { return image[n]; };
	inline void				setPixel( int n, int v ) { image[n] = v; };
	
	int						calculateChecksum();
	
	void					shift( int dX, int dY );
	void					setAllPixels( int v );
	void					fillRect( int x0, int y0, int x1, int y1, int v );
	void					drawLine( int x0, int y0, int x1, int y1, int v );
	
	bool					write( const char* fileName );
	bool					read( const char* fileName );

	unsigned char			*image;
	int						width, height;
	
	static bool				FLIP_VIDEO_VERT;
	static bool				FLIP_VIDEO_HORZ;
};


#endif