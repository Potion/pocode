#include "poCommon.h"

#ifndef _PO_IMAGE_H_
#define _PO_IMAGE_H_

class poTexture;

class poImage
{
public:
	poImage();
	poImage( int w, int h, int c );
	poImage(const char* filename);
	poImage(const char* buffer, int w, int h, int c);
	~poImage();
	
	void loadBuffer(const char* buffer, int w, int h, int c);
	void loadImage(const char* filename);
	
	void		loadIntoTexture( poTexture* tex );
	poTexture*	getTexture();
	
	int		width, height;
	int		components;
	bool	loadSuccess;

	inline void			setPixel( int i, int j, int c, unsigned char value ){ rawBuffer[ (j*width+i)*components + c ] = value; };
	void				setPixel( int i, int j, poColor C );
	unsigned char		getSubPixel( int i, int j, int c );
	poColor				getPixelColor( int i, int j );
	
	void				threshold( float thresholdLevel );
	void				blur( int blurRepeat );
	void				erode();
	void				dilate();
	
private:
#ifndef WIN32
	void loadImageRef(CFURLRef url);
	void loadPDFRef(CFURLRef url);
#endif

	poTexture* tex;
	unsigned char* rawBuffer;
};

#endif