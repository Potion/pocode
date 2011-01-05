#include "poVideoTexture.h"
#ifdef WIN32
#include <gl\gl.h>			
#endif

//static members
MovieDrawingCompleteUPP poVideoTexture::myDrawCompleteProc = NULL;

#ifdef WIN32
Rect * GetPixBounds(PixMapHandle pixMap, Rect *bounds)
{
  *bounds = (**pixMap).bounds;

  return bounds;
}
#endif

poVideoTexture::poVideoTexture()
: poTexture(), frame(NULL), isPlaying(false), doNewTextureBind(false),
  player(NULL), theMovie(NULL), theOffscreenGworld(NULL)
{
#ifdef WIN32
	//initialize the quicktime media layer
	InitializeQTML(0); 
#endif
	EnterMovies();

	movieFileName = "";
}

poVideoTexture::poVideoTexture(char* movieFileName)
:poTexture(), frame(NULL), isPlaying(false), doNewTextureBind(false),
player(NULL), theMovie(NULL), theOffscreenGworld(NULL)
{
#ifdef WIN32
	//initialize the quicktime media layer
	InitializeQTML(0); 
#endif
	EnterMovies();

	load(movieFileName);
}

poVideoTexture::~poVideoTexture()
{
	if(theMovie) //removie procedure
		::SetMovieDrawingCompleteProc(theMovie, 0, NULL, 0L);

	stop();

	frame = NULL;
	
	ExitMovies();
#ifdef WIN32
	TerminateQTML();
#endif
}

void poVideoTexture::loadMovieTexture()
{
	short       actualResId = DoTheRightThing;
	//printf( "poVideoTexture::init = %s\n", movieFileName );
#ifdef WIN32
	short theFile;
	FSSpec theFSSpec;			//file system spcecification record
    short  resRefNum = -1;		//unique id for this movie
#else
	OSType      myDataRefType;
	Handle      myDataRef = NULL;
	CFStringRef inPath;
#endif
    	
    stop();				//stop any previous movie and remove references

    myDrawCompleteProc = NewMovieDrawingCompleteUPP(DrawCompleteProc);

#ifdef WIN32
	//Windows platfor specific initializations
	OSErr result = NativePathNameToFSSpec( movieFileName, &theFSSpec, 0 /*flags*/);
	if (result) { return; }

	//get the unique id (resRefNum) for this movie)
	result = OpenMovieFile(&theFSSpec, &resRefNum, 0);
	if (result) { return; }

	//creates movie record in memory. set the rest of the params to 0 b/c they are Mac-specific
	result = NewMovieFromFile(&theMovie, resRefNum, &actualResId, (unsigned char *) 0, 0, (Boolean *) 0);
	if (result) { return; }

	if (resRefNum != -1) 
		CloseMovieFile(resRefNum);
#else
	OSErr		result = 0;
	
	inPath = CFStringCreateWithCString( NULL, movieFileName.c_str(), CFStringGetSystemEncoding() );
	if (!inPath)
	{
		printf("Could not get CFString \n");
		return;
	}
	
	// create the data reference
	result = QTNewDataReferenceFromFullPathCFString(inPath, kQTNativeDefaultPathStyle, 0, &myDataRef, &myDataRefType);
	if (result) { 
		printf("Could not get DataRef %d\n", result); 
		return;
	}
	
	// get the Movie
	result = NewMovieFromDataRef(&theMovie, newMovieActive,
								 &actualResId, myDataRef, myDataRefType);
	if (result) { 
		printf("Could not get Movie from DataRef %d\n", result); 
		return;
	}
	
	// dispose the data reference handle - we no longer need it
	DisposeHandle(myDataRef);
#endif

	//common to Windows and Mac OS
	Rect bounds;
	GetMovieBox(theMovie, &bounds);
	QTNewGWorld(&theOffscreenGworld, k32ARGBPixelFormat, &bounds, NULL, NULL, 0); 
	LockPixels(GetGWorldPixMap(theOffscreenGworld));
	SetGWorld(theOffscreenGworld, NULL); 
	
	player = NewMovieController(theMovie, &bounds, mcTopLeftMovie | mcNotVisible);
	SetMovieGWorld(theMovie, theOffscreenGworld, NULL);
	SetMovieActive(theMovie, true);
	//instead of the GWorld ref, pass a reference to the instance of the class
	SetMovieDrawingCompleteProc(theMovie, movieDrawingCallWhenChanged, poVideoTexture::myDrawCompleteProc, long(this));


	GetPixBounds(GetGWorldPixMap(theOffscreenGworld), &bounds);
	width = bounds.right - bounds.left;
	height = bounds.bottom - bounds.top;
}


void poVideoTexture::load( char* filename, bool preroll )
{
	movieFileName = filename;
	loadMovieTexture();
}


void poVideoTexture::play()
{	
    if (!theMovie) return;

	//preroll the movie (prepare audio/video and other channels needed for playback)
    MCDoAction(player, mcActionPrerollAndPlay, (void *)Long2Fix(1));
    isPlaying = true;
}

void poVideoTexture::rewind()
{	
    if (!theMovie) return;

	GoToBeginningOfMovie( theMovie );
}

void poVideoTexture::pause()
{	
    if (!theMovie) return;
		
    StopMovie( theMovie );
    isPlaying = false;
	
}

float poVideoTexture::getDuration()
{
	if (!theMovie) return 0;
	
	return (float)GetMovieDuration( theMovie )/float(GetMovieTimeScale(theMovie));
}

float poVideoTexture::getCurrentTime()
{
	if (!theMovie) return 0;
	
	//return GetMovieTime( theMovie,NULL );//GetMovieTimeBase( theMovie );
	return (float)GetMovieTime( theMovie,NULL )/float(GetMovieTimeScale(theMovie));
}

void poVideoTexture::stop()
{
    if (!theMovie) return;
	
	//clean up and remove any references

    GoToBeginningOfMovie( theMovie );
    StopMovie( theMovie );
    isPlaying = false;
	
	if (player)
		DisposeMovieController(player);
    if (theMovie)
		DisposeMovie(theMovie);
    if (myDrawCompleteProc)
		DisposeMovieDrawingCompleteUPP(myDrawCompleteProc);
	if (theOffscreenGworld)
		DisposeGWorld( theOffscreenGworld );
	
	player = NULL;
	theMovie = NULL;
	myDrawCompleteProc = NULL;
	theOffscreenGworld = NULL;
}

void poVideoTexture::blackoutFrame()
{
	if( theMovie ) BlackoutPixelArray( theMovie );
}

void poVideoTexture::progress()
{
	if ( theMovie != NULL && isPlaying )
	{
		if ( !IsMovieDone(theMovie) )
			MCIdle(player);
		else
			isPlaying = false;
	}
}


void poVideoTexture::setup(poObject* obj)
{
	progress();
	poTexture::setup(obj);
}

void poVideoTexture::captureAt( float t )
{
	// set time
	TimeRecord tr;
	tr.base 		= GetMovieTimeBase(theMovie);
	long total 		= GetMovieDuration(theMovie );
	long newPos 	= (long)((float)total * t);
	SetMovieTimeValue(theMovie, newPos);
	MoviesTask(theMovie,0);
	
	play();

	// get the information we need from the GWorld
	Rect		bounds;
	GetPixBounds(GetGWorldPixMap(theOffscreenGworld), &bounds);
	Ptr baseAddr = GetPixBaseAddr(GetGWorldPixMap(theOffscreenGworld));
	long rowBytes = QTGetPixMapHandleRowBytes(GetGWorldPixMap(theOffscreenGworld));
	width = bounds.right - bounds.left;
	height = bounds.bottom - bounds.top;
	
	printf("w:%d h:%d\n", width,height);
	
	if ( captureFrame == NULL ){
		captureFrame = (char*) malloc( sizeof( char) * width * height * 4 );
	}
	
	// for each row
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			short R, G, B;
			
			unsigned char* Image = (unsigned char*) baseAddr;
			R = Image[ rowBytes*y + x*4 + 1 ];
			G = Image[ rowBytes*y + x*4 + 2 ];
			B = Image[ rowBytes*y + x*4 + 3 ];
            
			captureFrame[ y*width*3 + x*3 + 0 ] = R;
			captureFrame[ y*width*3 + x*3 + 1 ] = G;
			captureFrame[ y*width*3 + x*3 + 2 ] = B;
		}
	}

	pause();
}

OSErr poVideoTexture::getPixelArray(Movie theMovie)
{
	if ( isPlaying ==false ) {
		return 0;
	}        
	doNewTextureBind = true;
        	
	// get the information we need from the GWorld
	Rect		bounds;
	GetPixBounds(GetGWorldPixMap(theOffscreenGworld), &bounds);
	Ptr baseAddr = GetPixBaseAddr(GetGWorldPixMap(theOffscreenGworld));
	long rowBytes = QTGetPixMapHandleRowBytes(GetGWorldPixMap(theOffscreenGworld));

	width = bounds.right - bounds.left;
	height = bounds.bottom - bounds.top;
	
	if ( frame == NULL )
		frame = ( char*) malloc( sizeof( char) * width * height * 4 );

	// for each row
	for (int y = 0; y < height; ++y) 
        {
            for (int x = 0; x < width; ++x) 
            {
                short R, G, B;
                
                unsigned char* Image = (unsigned char*) baseAddr;
                R = Image[ rowBytes*y + x*4 + 1 ];
                G = Image[ rowBytes*y + x*4 + 2 ];
                B = Image[ rowBytes*y + x*4 + 3 ];
            
                frame[ y*width*3 + x*3 + 0 ] = R;
                frame[ y*width*3 + x*3 + 1 ] = G;
                frame[ y*width*3 + x*3 + 2 ] = B;
            }
	}

	if(isPlaying) loadImage(width, height, 3,  (const unsigned char*)frame);

	return noErr;
}

OSErr poVideoTexture::BlackoutPixelArray(Movie theMovie)
{
	if ( isPlaying ==false ) {
		return 0;
	}        
	doNewTextureBind = true;
	
	// get the information we need from the GWorld
	Rect		bounds;
	GetPixBounds(GetGWorldPixMap(theOffscreenGworld), &bounds);
//	Ptr baseAddr = GetPixBaseAddr(GetGWorldPixMap(theOffscreenGworld));
//	long rowBytes = QTGetPixMapHandleRowBytes(GetGWorldPixMap(theOffscreenGworld));
	
	width = bounds.right - bounds.left;
	height = bounds.bottom - bounds.top;
	
	if ( frame == NULL )
		frame = ( char*) malloc( sizeof( char) * width * height * 4 );
	
	// for each row
	for (int y = 0; y < height; ++y) 
	{
		for (int x = 0; x < width; ++x) 
		{
			short R, G, B;
			
//			unsigned char* Image = (unsigned char*) baseAddr;
			R = 0;//Image[ rowBytes*y + x*4 + 1 ];
			G = 0;//Image[ rowBytes*y + x*4 + 2 ];
			B = 0;//Image[ rowBytes*y + x*4 + 3 ];
            
			frame[ y*width*3 + x*3 + 0 ] = R;
			frame[ y*width*3 + x*3 + 1 ] = G;
			frame[ y*width*3 + x*3 + 2 ] = B;
		}
	}
	
	if(isPlaying) loadImage(width, height, 3, (const unsigned char*)frame);
	
	return noErr;
}

//after quicktime processing, QT calls this function
//we're passing it a reference to an instance of this class
pascal OSErr poVideoTexture::DrawCompleteProc(Movie theMovie, long refCon)
{
    poVideoTexture *instance = reinterpret_cast<poVideoTexture*>(refCon);
    OSErr err = noErr;

    if (instance)
        err = instance->getPixelArray(theMovie);

    return err;
}

