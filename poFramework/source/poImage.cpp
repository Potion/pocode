#include "poImage.h"
#include "poTexture.h"

poImage::poImage()
{
	width = height = components = 0;
	tex = NULL;
	rawBuffer = NULL;
	loadSuccess = false;
}

poImage::poImage( int w, int h, int c )
{
	width = w;
	height = h;
	components = c;
	tex = NULL;
	loadSuccess = false;
	
	rawBuffer = (unsigned char*)malloc(w * h * c);
	memset(rawBuffer, 0, w * h * c);
}

poImage::poImage(const char* filename)
{
	width = height = components = 0;
	rawBuffer = NULL;
	tex = NULL;
	loadSuccess = false;
	loadImage(filename);
}

poImage::poImage(const char* buffer, int w, int h, int c)
{
	width = height = components = 0;
	rawBuffer = NULL;
	tex = NULL;
	loadSuccess = false;
	loadBuffer(buffer, w, h, c);
}

poImage::~poImage()
{
	if(rawBuffer != NULL)
		delete [] rawBuffer;
	if(tex != NULL)
		delete tex;
}

void poImage::loadBuffer(const char* buffer, int w, int h, int c)
{
	width = w;
	height = h;
	components = c;
	
	rawBuffer = (unsigned char*)realloc(rawBuffer, w * h * c);
	memcpy(rawBuffer, buffer, w * h * c);
}

poTexture* poImage::getTexture()
{
	if(tex == NULL) {
		tex = new poTexture();
		tex->loadImage(width, height, components, rawBuffer);
	}
	return tex;
}

void	poImage::loadIntoTexture( poTexture* aTex )
{
	aTex->loadImage(width, height, components, rawBuffer);
}

void poImage::setPixel( int i, int j, poColor C )
{
	rawBuffer[ (j*width+i)*components ] = (unsigned char)(C.R * 255);
	if( components > 1 )
		rawBuffer[ (j*width+i)*components + 1 ] = C.G * 255.0;
	if( components > 2 )
		rawBuffer[ (j*width+i)*components + 2 ] = C.B * 255.0;
	if( components > 3 )
		rawBuffer[ (j*width+i)*components + 3 ] = C.A * 255.0;
}

poColor poImage::getPixelColor( int i, int j )
{
	poColor C;
	if( components == 1 )
	{
		C.R = C.G = C.B = rawBuffer[ (j*width+i)*components ] / 255.0;
		C.A = 1.0;
	}
	else if( components == 2 )
	{
		C.R = C.G = C.B = rawBuffer[ (j*width+i)*components ] / 255.0;
		C.A = rawBuffer[ (j*width+i)*components + 1 ] / 255.0;
	}
	else if( components == 3 )
	{
		C.R = rawBuffer[ (j*width+i)*components ] / 255.0;
		C.G = rawBuffer[ (j*width+i)*components + 1 ] / 255.0;
		C.B = rawBuffer[ (j*width+i)*components + 2 ] / 255.0;
		C.A = 1.0;
	}
	else if( components == 4 )
	{
		C.R = rawBuffer[ (j*width+i)*components ] / 255.0;
		C.G = rawBuffer[ (j*width+i)*components + 1 ] / 255.0;
		C.B = rawBuffer[ (j*width+i)*components + 2 ] / 255.0;
		C.A = rawBuffer[ (j*width+i)*components + 3 ] / 255.0;
	}
	
	return C;
}

unsigned char	poImage::getSubPixel( int i, int j, int c )
{
	if ( i<0 || i>=width || j<0 || j>=height )
		return 0;
	
	return rawBuffer[ (j*width+i)*components + c ];
}

void poImage::threshold( float thresholdLevel )
{
	for( int i=0; i< width; i++ )
	{
		for( int j=0; j< height; j++ )
		{
			poColor C = getPixelColor( i, j );
			float pixelColor = ( C.R + C.G + C.B )/3.0f;
			if( pixelColor > thresholdLevel )
				setPixel( i, j, poColor::white );
			else
				setPixel( i, j, poColor::black );
		}
	}
}

void poImage::blur( int blurRepeat )
{
	// initially, just copy whole buffer, then do progresssive blurring
	unsigned char* tmpBuffer = (unsigned char*)malloc(width * height * components);
	memcpy( tmpBuffer, rawBuffer, width * height * components );
	
	// repeat more than once if desired
	for( int r=0; r<blurRepeat; r++ )
	{
		// do blurring
		int E = 1;
		for( int i=E; i<width-E; i++ )
		{
			for( int j=E; j<height-E; j++ )
			{
				for( int c=0; c<components; c++ )
				{
					int TL = tmpBuffer[ ( (j+E)*width + i-E )*components + c ];
					int T = tmpBuffer[ ( (j+E)*width + i )*components + c ];
					int TR = tmpBuffer[ ( (j+E)*width + i+E )*components + c ];
					
					int L = tmpBuffer[ ( (j)*width + i-E )*components + c ];
					int C = tmpBuffer[ ( (j)*width + i )*components + c ];
					int R = tmpBuffer[ ( (j)*width + i+E )*components + c ];
					
					int BL = tmpBuffer[ ( (j-E)*width + i-E )*components + c ];
					int B = tmpBuffer[ ( (j-E)*width + i )*components + c ];
					int BR = tmpBuffer[ ( (j-E)*width + i+E )*components + c ];
					
					float blurSum = (TL+2*L+BL) + (T+2*C+B) + (TR+2*R+BR);
					blurSum /= 12.0;

					setPixel( i, j, c, blurSum);
				}
			}
		}
		
		// if blurring again, copy into tmpBuffer
		if ( r+1 < blurRepeat )
			memcpy( tmpBuffer, rawBuffer, width * height * components );
	}

	free( tmpBuffer );
}

void poImage::erode()
{
	unsigned char* tmpBuffer = (unsigned char*)malloc(width * height * components);
	memcpy( tmpBuffer, rawBuffer, width * height * components );

	int E = 1;
	for( int i=E; i<width-E; i++ )
	{
		for( int j=E; j<height-E; j++ )
		{
			for( int c=0; c<components; c++ )
			{
				int TL = tmpBuffer[ ( (j+E)*width + i-E )*components + c ];
				int T = tmpBuffer[ ( (j+E)*width + i )*components + c ];
				int TR = tmpBuffer[ ( (j+E)*width + i+E )*components + c ];
				
				int L = tmpBuffer[ ( (j)*width + i-E )*components + c ];
				int C = tmpBuffer[ ( (j)*width + i )*components + c ];
				int R = tmpBuffer[ ( (j)*width + i+E )*components + c ];
				
				int BL = tmpBuffer[ ( (j-E)*width + i-E )*components + c ];
				int B = tmpBuffer[ ( (j-E)*width + i )*components + c ];
				int BR = tmpBuffer[ ( (j-E)*width + i+E )*components + c ];
				
				int surround = TL & T & TR & L & R & BL & B & BR;
				if ( surround == 0 )
					setPixel( i,j, poColor::black );
			}
		}
	}
	
	free( tmpBuffer );
}

void poImage::dilate()
{
	unsigned char* tmpBuffer = (unsigned char*)malloc(width * height * components);
	memcpy( tmpBuffer, rawBuffer, width * height * components );
	
	int E = 1;
	for( int i=E; i<width-E; i++ )
	{
		for( int j=E; j<height-E; j++ )
		{
			//for( int c=0; c<components; c++ )
			int c = 0;
			{
				int TL = tmpBuffer[ ( (j+E)*width + i-E )*components + c ];
				int T = tmpBuffer[ ( (j+E)*width + i )*components + c ];
				int TR = tmpBuffer[ ( (j+E)*width + i+E )*components + c ];
				
				int L = tmpBuffer[ ( (j)*width + i-E )*components + c ];
				int C = tmpBuffer[ ( (j)*width + i )*components + c ];
				int R = tmpBuffer[ ( (j)*width + i+E )*components + c ];
				
				int BL = tmpBuffer[ ( (j-E)*width + i-E )*components + c ];
				int B = tmpBuffer[ ( (j-E)*width + i )*components + c ];
				int BR = tmpBuffer[ ( (j-E)*width + i+E )*components + c ];
				
				int surround = TL | T | TR | L | R | BL | B | BR;
				if ( surround )
					setPixel( i,j, poColor::white );
			}
		}
	}
	
	free( tmpBuffer );
}

// ========================== WINDOWS IMAGE LOADING ============================ //
#ifdef WIN32

	#include "FreeImage.h"
	

	/**
		FreeImage error handler
		@param fif Format / Plugin responsible for the error 
		@param message Error message
	*/
	void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
		printf("\n*** "); 
		if(fif != FIF_UNKNOWN) {
			printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
		}
		printf(message);
		printf(" ***\n");
	}

	unsigned DLL_CALLCONV
	myReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
		return fread(buffer, size, count, (FILE *)handle);
	}

	unsigned DLL_CALLCONV
	myWriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
		return fwrite(buffer, size, count, (FILE *)handle);
	}

	int DLL_CALLCONV
	mySeekProc(fi_handle handle, long offset, int origin) {
		return fseek((FILE *)handle, offset, origin);
	}

	long DLL_CALLCONV
	myTellProc(fi_handle handle) {
		return ftell((FILE *)handle);
	}


bool freeImageInited = false;

void poImage::loadImage(const char* filename)
{
	// call this ONLY when linking with FreeImage as a static library
	//#ifdef FREEIMAGE_LIB
		if ( freeImageInited == false )
		{
			FreeImage_Initialise();
			freeImageInited = true;
			printf("\n");
			// print version & copyright infos
			printf(FreeImage_GetVersion());
			printf(FreeImage_GetCopyrightMessage());
			printf("\n");
		}
	//#endif // FREEIMAGE_LIB


	FILE *file = fopen(filename, "rb");

	if ( file==NULL )
	{
		printf("could not load %s file\n", filename );
		loadSuccess = false;
		return;
	}
	if (file != NULL) {
		FreeImage_SetOutputMessage(FreeImageErrorHandler);

		FreeImageIO io;

		io.read_proc  = myReadProc;
		io.write_proc = myWriteProc;
		io.seek_proc  = mySeekProc;
		io.tell_proc  = myTellProc;

		// find the buffer format
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle(&io, (fi_handle)file, 0);

		if(fif != FIF_UNKNOWN) {
			// load from the file handle
			FIBITMAP *dib = FreeImage_LoadFromHandle(fif, &io, (fi_handle)file, 0);
			
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			
			printf("image %s size %d %d\n", filename, width, height );

			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
			components = bytespp;

			//printf("components %d\n",components);
			rawBuffer = (unsigned char*)malloc(components * width * height);

			// printf("%d %d %d\n", width, height, components );

			for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) 
			{
				BYTE *bits = FreeImage_GetScanLine(dib, y);
				for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) 
				{
					int idx1 = (height-y-1) * (width * components) + (x * components);
					rawBuffer[ idx1 ] = bits[FI_RGBA_RED];
					if(components > 1)
						rawBuffer[ idx1+1 ] = bits[FI_RGBA_GREEN];
					if(components > 2)
						rawBuffer[ idx1+2 ] = bits[FI_RGBA_BLUE];
					if(components > 3)
						rawBuffer[ idx1+3 ] = bits[FI_RGBA_ALPHA];

					// jump to next pixel
					bits += bytespp;
				}
			}

			// free the loaded FIBITMAP
			FreeImage_Unload(dib);
		}
		fclose(file);
	}

	// call this ONLY when linking with FreeImage as a static library
	//#ifdef FREEIMAGE_LIB
	//	FreeImage_DeInitialise();
	//#endif // FREEIMAGE_LIB

	loadSuccess = true;
}

#endif
#ifndef WIN32

#include "FreeImage.h"

// ========================== MAC OSX IMAGE LOADING ============================ //

void poImage::loadImage(const char* filename)
{
	
	CFStringRef name = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
	CFURLRef url = CFURLCreateWithFileSystemPath(NULL, name, kCFURLPOSIXPathStyle, false);
	CFRelease(name);

	FSRef ref;
	if(!CFURLGetFSRef(url, &ref))
	{
		printf("ERROR: poImage->loadImage: url doesn't exist (%s)\n", filename);
		loadSuccess = false;
		return;
	}
	
	LSItemInfoRecord outInfo;
	outInfo.extension = NULL;
	
	if(!LSCopyItemInfoForRef(&ref, kLSRequestExtension|kLSRequestTypeCreator, &outInfo))
	{
		// we'll assume this file has an extention
		CFStringRef uti = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, outInfo.extension, NULL);
		CFRelease(outInfo.extension);
		
		if(UTTypeConformsTo(uti, CFSTR("public.image")))
			loadImageRef(url);
		else if(UTTypeConformsTo(uti, CFSTR("com.adobe.pdf")))
			loadPDFRef(url);
		else 
		{
			printf("ERROR: poImage->loadImage: not an image format (%s)\n", filename);
			loadSuccess = false;
			return;
		}
		
		if ( loadSuccess == false )
			return;
	} else
	{
		printf("ERROR: poImage->loadImage: unknown file extension (%s)\n", filename);
		loadSuccess = false;
		return;
	}

	CFRelease(url);
	loadSuccess = true;
}

void poImage::loadImageRef(CFURLRef url)
{
	CGImageSourceRef sourceRef = CGImageSourceCreateWithURL(url, NULL);

	// this should be zero if source is ok
	CGImageSourceStatus status = CGImageSourceGetStatus(sourceRef);
	if(status != 0)
	{
		printf("ERROR: poImage->loadImage: img source is bad\n");
		loadSuccess = false;
		return;
	}

	CGImageRef imageRef = CGImageSourceCreateImageAtIndex(sourceRef, 0, NULL);

	width = CGImageGetWidth(imageRef);
	height = CGImageGetHeight(imageRef);
	components = CGImageGetBitsPerPixel(imageRef) / CGImageGetBitsPerComponent(imageRef);
	
	// setup the context
	UInt32* imageData = (UInt32*)malloc(sizeof(UInt32) * width * height);
	CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
	CGContextRef bitmapContext = CGBitmapContextCreate(imageData, width, height, 8, sizeof(UInt32)*width, colorSpace, kCGImageAlphaPremultipliedLast);
	if(bitmapContext == NULL)
	{
		printf("ERROR: poImage->loadImage: can't create bitmap context\n");
		loadSuccess = false;
		return;
	}
	// draw image into context
	CGRect imageRect = CGRectMake(0, 0, width, height);
	CGContextDrawImage(bitmapContext, imageRect, imageRef);

	rawBuffer = (unsigned char*)malloc(components * width * height);
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			int idx1 = y * (width * components) + (x * components);
			int idx2 = y * width + x;
			int r = (imageData[idx2] & 0xFF000000) >> 24;
			int g = (imageData[idx2] & 0x00FF0000) >> 16;
			int b = (imageData[idx2] & 0x0000FF00) >> 8;
			int a = (imageData[idx2] & 0x000000FF);

#if BYTE_ORDER == LITTLE_ENDIAN
			int store = a;
			a = r;
			r = store;
			
			store = b;
			b = g;
			g = store;
#endif
			float adiv = (float) a / 255.f;
			if(adiv == 0) adiv = 1;
			
			*(rawBuffer+idx1) = clamp(r / adiv, 0.f, 255.f);
			if(components > 1)
				*(rawBuffer+idx1 + 1) = clamp(g / adiv, 0.f, 255.f);
			if(components > 2)
				*(rawBuffer+idx1 + 2) = clamp(b / adiv, 0.f, 255.f);
			if(components > 3)
				*(rawBuffer+idx1 + 3) = clamp(a, 0, 255);
		}
	
	CGContextRelease(bitmapContext);
	CGColorSpaceRelease(colorSpace);
	free(imageData);

	CFRelease(sourceRef);
	CGImageRelease(imageRef);
	
	loadSuccess = true;
}

void poImage::loadPDFRef(CFURLRef url)
{
	CGPDFDocumentRef document = CGPDFDocumentCreateWithURL(url);
	int count = CGPDFDocumentGetNumberOfPages(document);
	if(count == 0) 
	{
		printf("ERROR: poImage->loadPDFRef: no pages in document\n");
		loadSuccess = false;
		return;
	}
	if(count > 1)
		printf("WARNING: poImage->loadPDFRef: only loading first page of %d\n", count);
		
	CGPDFPageRef page = CGPDFDocumentGetPage(document, 1);
	CGRect box = CGPDFPageGetBoxRect(page, kCGPDFTrimBox);
	
	width = box.size.width;
	height = box.size.height;
	components = 4;

	UInt32* imageData = (UInt32*)malloc(sizeof(UInt32) * width * height);
	CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
	CGContextRef bitmapContext = CGBitmapContextCreate(imageData, width, height, 8, sizeof(UInt32)*width, colorSpace, kCGImageAlphaPremultipliedLast);
	if(bitmapContext == NULL)
	{
		printf("ERROR: poImage->loadPDFRef: can't create bitmap context\n");
		loadSuccess = false;
		return;
	}
	// draw image into context
	CGContextDrawPDFPage(bitmapContext, page);
	
	rawBuffer = (unsigned char*)malloc(width * height * components);
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			int idx1 = y * (width * components) + (x * components);
			int idx2 = y * width + x;
			int r = (imageData[idx2] & 0xFF000000) >> 24;
			int g = (imageData[idx2] & 0x00FF0000) >> 16;
			int b = (imageData[idx2] & 0x0000FF00) >> 8;
			int a = (imageData[idx2] & 0x000000FF);

#if BYTE_ORDER == LITTLE_ENDIAN
			int store = a;
			a = r;
			r = store;
			
			store = b;
			b = g;
			g = store;
#endif
			float adiv = (float) a / 255.f;
			if(adiv == 0) adiv = 1;
			
			*(rawBuffer+idx1) = clamp(r / adiv, 0.f, 255.f);
			if(components > 1)
				*(rawBuffer+idx1 + 1) = clamp(g / adiv, 0.f, 255.f);
			if(components > 2)
				*(rawBuffer+idx1 + 2) = clamp(b / adiv, 0.f, 255.f);
			if(components > 3)
				*(rawBuffer+idx1 + 3) = clamp(a, 0, 255);
		}
	
	CGContextRelease(bitmapContext);
	CGColorSpaceRelease(colorSpace);
	free(imageData);

	CGPDFDocumentRelease(document);
	
	loadSuccess = true;
}

#endif