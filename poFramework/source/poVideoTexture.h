#include "poTexture.h"

#ifndef _PO_VIDEO_TEXTURE_H_
#define _PO_VIDEO_TEXTURE_H_

#ifdef WIN32
	#include "QuicktimeWindowsHeaders.h"
#else
	#include <QuickTime/QuickTime.h>
	#include <GLUT/glut.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif

class poObject;

class poVideoTexture : public poTexture
{
public:
	poVideoTexture();
	poVideoTexture( char* movieFile );
	~poVideoTexture();

	virtual const char*			getClassName() { return "poVideoTexture"; };
	virtual poObjectModifier*	duplicate() { return new poVideoTexture(); };
	
	virtual void setup(poObject* obj); 
	
	void load( char* movieFileName, bool preroll = true );	//loads a new movie in the same texture
																//NOTE: if you preroll and wait a while before play, you will miss the beginning frames
	void    loadMovieTexture();

	//Quicktime movie stuff
	void					play();							//prepares channels needed for playback 
															//and sets it to play at desired rate
	void					stop();							//stops movie and releases references (NOT A PAUSE)
	void					progress();						//progresses the movie frame by frame

	void					pause();						//pauses the movie movie
	void					rewind();						//restarts the movie movie	
	
	float					getDuration();
	float					getCurrentTime();	
	
	void					blackoutFrame();
	
	bool					isStillPlaying() { return isPlaying; };

	char*					frame;
	std::string				movieFileName;

	// young added 071610
	void					captureAt( float t );
	char*					captureFrame;
private:

    static pascal OSErr		DrawCompleteProc(Movie theMovie, long refCon);
    static					MovieDrawingCompleteUPP myDrawCompleteProc;

	OSErr					getPixelArray(Movie theMovie);
	OSErr					BlackoutPixelArray(Movie theMovie);

	bool					isPlaying;
	bool					doNewTextureBind;

	MovieController			player;
	Movie					theMovie;
	GWorldPtr				theOffscreenGworld;
};



#endif