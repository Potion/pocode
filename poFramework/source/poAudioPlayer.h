/*
 *  poAudio.h
 *  poFramework_static_lib
 *
 *  Created by Young Cho on 8/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poObject.h"
//#include <alut.h>

enum	poFadeMode{ PO_FADE_IN, PO_FADE_OUT, PO_FADE_NONE };

class poAudioProperty
{
public:
	poAudioProperty(){
		soundID = 0;
		LOOP = false;
		PITCH = 1.0;
		GAIN = 1.0;
		MIN_GAIN = MAX_GAIN = 1.0;
		FADEMODE = PO_FADE_NONE;
		FADESPEED = 0.1;
		pos.set( 0,0,0 );
		vel.set( 0,0,0 );
	};

	ALuint				soundID;
	bool				LOOP;
	float				PITCH;
	float				GAIN, MIN_GAIN, MAX_GAIN;
	poFadeMode			FADEMODE;
	float				FADESPEED;
	poPoint				pos;
	poPoint				vel;
};

class poAudioPlayer : public poObject
{
public:
	poAudioPlayer();
	
	virtual void				update();
	
	void						initAudio();
	static poAudioPlayer*		getAudioPlayer();
	void						addSound( char* filename );
	
	ALuint						play( int S, bool LOOP = false, float GAIN = 1.0 );
	ALuint						play( int S, poAudioProperty &P );
	void						stop( ALuint soundID );
	void						stopAll();
	void						pause( ALuint soundID );
	void						resume( ALuint soundID );
	
	void						setListenerPos( float x, float y, float z );
	poAudioProperty				getAudioProperty( ALuint soundID );
	void						setAudioProperty( ALuint soundID, poAudioProperty &P );
	
	static int					NUM_BUFFERS;
	static int					NUM_SOURCES;
	
	static poAudioPlayer*		audioPlayer;
	
	static ALuint*				buffer;
	
	ALfloat						listenerPos [3];
	ALfloat						listenerVel [3];
	ALfloat						listenerOri [6];
	
	static std::vector<poAudioProperty*>	sourceList;
	
private:
	ALuint						playBuffer( int S, poAudioProperty &P );
};